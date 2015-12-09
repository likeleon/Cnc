#include "cnc/stdafx.h"
#include "cnc/game.h"
#include "cnc/log.h"
#include "cnc/platform.h"
#include "cnc/settings.h"
#include "cnc/global_file_system.h"
#include "cnc/debug.h"
#include "cnc/renderer.h"
#include "cnc/perf_sample.h"
#include "cnc/perf_history.h"

namespace cnc {

std::unique_ptr<Settings> Game::settings_;
std::unique_ptr<Renderer> Game::renderer_;
StopWatch Game::stop_watch_;
RunStatus Game::state_ = RunStatus::Running;
int32_t Game::render_frame_ = 0;

void Game::Initialize(const Arguments& args) {
  std::cout << "Platform is " << Platform::CurrentPlatform() << std::endl;

  InitializeSettings(args);

  Log::AddChannel("perf", "perf.log");
  Log::AddChannel("debug", "debug.log");
  Log::AddChannel("graphics", "graphics.log");

  GlobalFileSystem::Mount(Platform::GameDir());
  std::vector<std::string> renderers{ settings_->graphics().renderer, "Default", "" };
  for (const auto& r : renderers) {
    if (r.empty()) {
      Debug::Error("No suitable renderes were found. Check graphics.log for details.");
    }

    settings_->graphics().renderer = r;
    
    renderer_ = std::make_unique<Renderer>(settings_->graphics());
    if (renderer_ != nullptr) {
      break;
    }
  }
}

void Game::InitializeSettings(const Arguments& args) {
  std::vector<std::string> paths{ "^", "settings.yaml " };
  settings_ = std::make_unique<Settings>(Platform::ResolvePath(paths), args);
}

RunStatus Game::Run() {
  Loop();
  renderer_.reset();
  return state_;
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
    int32_t max_framerate = 1000;
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

    renderer_->BeginFrame();

    PERF_SAMPLE(render_flip, {
      renderer_->EndFrame();
    });
  })

  PerfHistory::Item("render").Tick();
  PerfHistory::Item("render_flip").Tick();
}

}