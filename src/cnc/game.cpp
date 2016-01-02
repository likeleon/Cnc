#include "cnc/stdafx.h"
#include "cnc/game.h"
#include "cnc/log.h"
#include "cnc/platform.h"
#include "cnc/settings.h"
#include "cnc/file_system.h"
#include "cnc/error.h"
#include "cnc/renderer.h"
#include "cnc/perf_sample.h"
#include "cnc/perf_history.h"
#include "cnc/input_handler.h"
#include "cnc/mod_metadata.h"
#include "cnc/mod_data.h"
#include "cnc/widget.h"

namespace cnc {

std::unique_ptr<Settings> Game::settings_;
std::unique_ptr<Renderer> Game::renderer_;
StopWatch Game::stop_watch_;
RunStatus Game::state_ = RunStatus::Running;
int32_t Game::render_frame_ = 0;
std::unique_ptr<ModData> Game::mod_data_;

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

void Game::InitializeMod(const std::string& mod, const Arguments& args) {
  Ui::ResetAll();

  mod_data_.reset();

  std::cout << "Loading mod: " << mod << std::endl;
  settings_->game().mod = mod;

  mod_data_ = std::make_unique<ModData>(mod, true);

  mod_data_->MountFiles();
  mod_data_->InitializeLoaders();
  renderer_->InitializeFonts(mod_data_->manifest());

  PerfHistory::Items("render").set_has_normal_tick(false);
  PerfHistory::Items("batches").set_has_normal_tick(false);
  PerfHistory::Items("render_widgets").set_has_normal_tick(false);
  PerfHistory::Items("render_flip").set_has_normal_tick(false);

  mod_data_->load_screen()->StartGame(args);
}

RunStatus Game::Run() {
  Loop();
  renderer_.reset();
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

        // LogicTick();
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

void Game::RenderTick() {
  PERF_SAMPLE(render, {
    ++render_frame_;

    renderer_->BeginFrame(Point::Zero, 1.0f);

    PERF_SAMPLE(render_widgets, {
      Ui::PrepareRenderables();
      Ui::Draw();
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

}