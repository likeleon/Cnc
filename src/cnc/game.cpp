#include "cnc/stdafx.h"
#include "cnc/game.h"
#include "cnc/log.h"
#include "cnc/platform.h"

namespace cnc {

void Game::Initialize(const Arguments& args) {
  std::cout << "Platform is " << Platform::GetCurrentPlatform() << std::endl;

  InitializeSettings(args);

  Log::AddChannel("perf", L"perf.log");
}

void Game::InitializeSettings(const Arguments& /*args*/) {
  std::vector<std::wstring> paths{ L"^", L"settings.yaml" };
  auto settings_file = Platform::ResolvePath(paths);
}

RunStatus Game::Run() {
  return RunStatus::Success;
}

}