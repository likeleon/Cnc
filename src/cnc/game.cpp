#include "cnc/stdafx.h"
#include "cnc/game.h"
#include "cnc/log.h"
#include "cnc/platform.h"
#include "cnc/settings.h"

namespace cnc {

std::unique_ptr<Settings> Game::settings_;

void Game::Initialize(const Arguments& args) {
  std::cout << "Platform is " << Platform::GetCurrentPlatform() << std::endl;

  InitializeSettings(args);

  Log::AddChannel("perf", "perf.log");
}

void Game::InitializeSettings(const Arguments& args) {
  std::vector<std::string> paths{ "^", "settings.yaml " };
  settings_ = std::make_unique<Settings>(Platform::ResolvePath(paths), args);
}

RunStatus Game::Run() {
  return RunStatus::Success;
}

}