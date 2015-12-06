#include "cnc/stdafx.h"
#include "cnc/game.h"
#include "cnc/log.h"
#include "cnc/platform.h"
#include "cnc/settings.h"
#include "cnc/global_file_system.h"
#include "cnc/debug.h"
#include "cnc/renderer.h"

namespace cnc {

std::unique_ptr<Settings> Game::settings_;
std::unique_ptr<Renderer> Game::renderer_;

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
  return RunStatus::Success;
}

}