#include "cnc/stdafx.h"
#include "cnc/game.h"
#include "cnc/log.h"
#include "cnc/platform.h"
#include "cnc/settings.h"
#include "cnc/error.h"
#include "cnc/renderer.h"
#include "cnc/perf_sample.h"
#include "cnc/perf_history.h"
#include "cnc/input_handler.h"
#include "cnc/mod_metadata.h"
#include "cnc/mod_data.h"
#include "cnc/widget.h"
#include "cnc/software_cursor.h"
#include "cnc/viewport.h"
#include "cnc/action_queue.h"
#include "cnc/iload_screen.h"
#include "cnc/content_installer.h"
#include "cnc/file.h"
#include "cnc/arguments.h"
#include "cnc/download.h"
#include "cnc/perf_timer.h"
#include "cnc/world.h"
#include "cnc/map_cache.h"
#include "cnc/map_preview.h"
#include "cnc/map.h"
#include <random>

namespace cnc {

std::unique_ptr<Settings> Game::settings_;
std::unique_ptr<ICursor> Game::cursor_;
std::unique_ptr<Renderer> Game::renderer_;
StopWatch Game::stop_watch_;
RunStatus Game::state_ = RunStatus::Running;
int32_t Game::render_frame_ = 0;
std::unique_ptr<ModData> Game::mod_data_;
Modifiers Game::modifiers_ = Modifiers::None;
std::unique_ptr<ActionQueue> Game::delayed_actions_;

void Game::Initialize(const Arguments& args) {
  std::cout << "Platform is " << Platform::CurrentPlatform() << std::endl;

  InitializeSettings(args);

  Log::AddChannel("perf", "perf.log");
  Log::AddChannel("debug", "debug.log");
  Log::AddChannel("graphics", "graphics.log");

  std::vector<std::string> renderers{ settings_->graphics().renderer, "Default", "" };
  for (const auto& r : renderers) {
    if (r.empty()) {
      throw Error(MSG("No suitable renderes were found. Check graphics.log for details."));
    }

    settings_->graphics().renderer = r;
    
    renderer_ = std::make_unique<Renderer>(settings_->graphics());
    if (renderer_ != nullptr) {
      break;
    }
  }

  std::cout << "Available mods:" << std::endl;
  for (const auto& kv : ModMetadata::AllMods()) {
    std::cout << "\t" << kv.first << ": " + kv.second.title << " (" + kv.second.version + ")" << std::endl;
  }

  InitializeMod(settings_->game().mod, args);
}

void Game::InitializeSettings(const Arguments& args) {
  std::vector<std::string> paths{ "^", "settings.yaml " };
  settings_ = std::make_unique<Settings>(Platform::ResolvePaths(paths), args);
}

bool Game::IsModInstalled(std::pair<std::string, std::string> mod) {
  const auto& all_mods = ModMetadata::AllMods();
  return all_mods.find(mod.first) != all_mods.end() &&
         all_mods.at(mod.first).version == mod.second &&
         IsModInstalled(mod.first);
}

bool Game::IsModInstalled(const std::string& mod_id) {
  const auto& required_mods = Manifest::AllMods().at(mod_id).requires_mods();
  return std::all_of(required_mods.begin(), required_mods.end(),
                     [](const auto& m) { return IsModInstalled(m); });
}

void Game::InitializeMod(const std::string& m, const Arguments& args) {
  delayed_actions_ = std::make_unique<ActionQueue>();

  Ui::ResetAll();

  if (mod_data_ != nullptr) {
    mod_data_->mod_files().UnmountAll();
    mod_data_ = nullptr;
  }

  std::string mod = m;
  const auto& all_mods = ModMetadata::AllMods();
  if (all_mods.find(mod) == all_mods.end() || !IsModInstalled(mod)) {
    mod = GameSettings().mod;
  }

  std::cout << "Loading mod: " << mod << std::endl;
  settings_->game().mod = mod;

  mod_data_ = std::make_unique<ModData>(mod, true);

  PERF_TIMER("LoadMaps", {
    mod_data_->map_cache().LoadMaps();
  });

  auto& install_data = mod_data_->manifest().Get<ContentInstaller>();
  auto is_mod_content_installed = std::all_of(
    install_data.test_files.begin(), 
    install_data.test_files.end(),
    [](const std::string& file) { return File::Exists(Platform::ResolvePath(file));
  });

  if (!is_mod_content_installed) {
    InitializeMod("modchooser", Arguments::Empty);
    return;
  }
  
  mod_data_->MountFiles();
  mod_data_->InitializeLoaders();
  renderer_->InitializeFonts(mod_data_->manifest());

  cursor_ = nullptr;

  // TODO: hardware cursor
  cursor_ = std::make_unique<SoftwareCursor>(*mod_data_->cursor_provider());

  PerfHistory::Items("render").set_has_normal_tick(false);
  PerfHistory::Items("batches").set_has_normal_tick(false);
  PerfHistory::Items("render_widgets").set_has_normal_tick(false);
  PerfHistory::Items("render_flip").set_has_normal_tick(false);

  mod_data_->load_screen()->StartGame(args);
}

void Game::LoadShellMap() {
  auto shellmap = ChooseShellmap();

  PERF_TIMER("StartGame", {
    StartGame(shellmap, WorldType::Shellmap);
  });
}

std::string Game::ChooseShellmap() {
  std::vector<std::string> shellmaps;
  for (const auto& m : mod_data_->map_cache().Previews()) {
    if (m->status() == MapStatus::Available && 
        (m->map()->visibility() & MapVisibility::Shellmap) != 0) {
      shellmaps.emplace_back(m->uid());
    }
  }

  std::default_random_engine gen(static_cast<uint32_t>(time(0)));
  std::uniform_int_distribution<> dis(0, shellmaps.size() - 1);
  return shellmaps[dis(gen)];
}

void Game::StartGame(const std::string& /*map_uid*/, WorldType /*type*/) {
  // TODO
}

RunStatus Game::Run() {
  Loop();
  renderer_ = nullptr;
  return state_;
}

void Game::Exit() {
  state_ = RunStatus::Success;
}

int64_t Game::RunTime() {
  return stop_watch_.ElapsedMilliseconds();
}

void Game::Loop() {
  const int32_t MaxLogicTicksBehind = 250;

  int64_t next_logic = RunTime();
  int64_t next_render = RunTime();
  int64_t forced_next_render = RunTime();

  while (state_ == RunStatus::Running) {
    int64_t logic_interval = Timestep;
    int32_t max_framerate = 60;
    int32_t render_interval = 1000 / max_framerate;

    auto now = RunTime();

    if (now - next_logic > MaxLogicTicksBehind) {
      next_logic = now;
    }

    auto next_update = std::min(next_logic, next_render);
    if (now >= next_update) {
      bool force_render = now >= forced_next_render;

      if (now >= next_logic) {
        next_logic += logic_interval;

        LogicTick();
      }

      bool have_some_time_until_next_logic = now < next_logic;
      bool is_time_to_render = now >= next_render;

      if ((is_time_to_render && have_some_time_until_next_logic) || force_render) {
        next_render = now + render_interval;

        forced_next_render = now + render_interval;

        RenderTick();
      }
    } else {
      Platform::Sleep(next_update - now);
    }
  }
}

void Game::LogicTick() {
  delayed_actions_->PerformActions(Game::RunTime());
}

void Game::RenderTick() {
  PERF_SAMPLE(render, {
    ++render_frame_;

    renderer_->BeginFrame(Point::Zero, 1.0f);

    PERF_SAMPLE(render_widgets, {
      Ui::PrepareRenderables();
      Ui::Draw();

      if (mod_data_ != nullptr && mod_data_->cursor_provider() != nullptr) {
        std::string cursor_name = Ui::root()->GetCursorOuter(Viewport::last_mouse_pos_);
        if (cursor_name.empty()) {
          cursor_name = "default";
        }
        cursor_->SetCursor(cursor_name);
        cursor_->Render(*renderer_);
      }
    });

    PERF_SAMPLE(render_flip, {
      DefaultInputHandler input_handler;
      renderer_->EndFrame(input_handler);
    });
  })

  PerfHistory::Items("render").Tick();
  PerfHistory::Items("batches").Tick();
  PerfHistory::Items("render_widgets").Tick();
  PerfHistory::Items("render_flip").Tick();
}

Renderer* Game::renderer() {
  return renderer_.get();
}

ModData* Game::mod_data() {
  return mod_data_.get();
}

Settings& Game::settings() {
  return *settings_;
}

Modifiers Game::GetModifierKeys() {
  return modifiers_;
}

void Game::HandleModifierKeys(Modifiers mods) {
  modifiers_ = mods;
}

void Game::RunAfterTick(const std::function<void()>& a) {
  delayed_actions_->Add(a, Game::RunTime());
}

}