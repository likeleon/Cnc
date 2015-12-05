#include "cnc/stdafx.h"
#include "cnc/arguments.h"
#include "cnc/game.h"
#include "cnc/log.h"

using namespace cnc;

static std::ostringstream BuildExceptionReport(const std::exception& e) {
  std::ostringstream oss;
  oss << "Exception of type '" << typeid(e).name() << "': " << e.what();
  return oss;
}

static void FatalError(const std::exception& e) {
  Log::AddChannel("exception", "exception.log");
  std::string report = BuildExceptionReport(e).str();
  std::cerr << report.c_str();
}

static RunStatus Run(int32_t argc, char** argv) {
  Game::Initialize(Arguments(argc, argv));
  return Game::Run();
}

int32_t main(int32_t argc, char** argv) {
  try {
    return (int32_t)Run(argc, argv);
  } catch (const std::exception& e) {
    FatalError(e);
    return static_cast<int32_t>(RunStatus::Error);
  }
}