#include "cnc/stdafx.h"
#include "cnc/game.h"
#include "cnc/log.h"
#include "cnc/platform.h"

namespace cnc {

void Game::Initialize(const Arguments& /*args*/) {
  std::cout << "Platform is " << Platform::GetCurrentPlatform();

  Log::AddChannel("perf", "perf.log");
}

RunStatus Game::Run() {
  return RunStatus::Success;
}

}