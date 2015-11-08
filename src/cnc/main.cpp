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
  Log::AddChannel("test", "");
  Log::AddChannel("exception", "exception.log");
  std::string report = BuildExceptionReport(e).str();
  std::cerr << report.c_str();
}

static RunStatus Run(int argc, char** argv) {
  Game::Initialize(Arguments(argc, argv));
  return Game::Run();
}

int main(int argc, char** argv) {
  try {
    return (int)Run(argc, argv);
  } catch (const std::exception& e) {
    FatalError(e);
    return static_cast<int>(RunStatus::Error);
  }
}