#include "cnc/game/stdafx.h"
#include "cnc/arguments.h"
#include "cnc/game.h"
#include "cnc/log.h"
#include "cnc/mod_data.h"
#include "cnc/platform.h"

using namespace cnc;

template <typename T>
static std::ostringstream BuildExceptionReport(const T& e);

template <>
std::ostringstream BuildExceptionReport(const std::exception& e) {
  std::ostringstream oss;
  oss << "Exception of type '" << typeid(e).name() << "': " << e.what();
  return oss;
}

template <>
std::ostringstream BuildExceptionReport(const Error& e) {
  std::ostringstream oss;
  oss << "Exception of type '" << typeid(e).name() << "': \n" << e;
  return oss;
}

template <typename T>
static void FatalError(const T& e) {
  Log::AddChannel("exception", "exception.log");

  if (Game::mod_data() != nullptr) {
    const auto& mod = Game::mod_data()->manifest().mod();
    std::ostringstream oss;
    oss << mod.title << " Mod at Version " << mod.version;
    Log::Write("exception", oss.str());
  }

  std::ostringstream oss;
  oss << "Operating System: " << Platform::CurrentPlatform();
  Log::Write("exception", oss.str());
  
  std::string report = BuildExceptionReport(e).str();
  Log::Write("exception", report);
  std::cerr << report.c_str() << std::endl;
}

static RunStatus Run(int32_t argc, char** argv) {
  Game::Initialize(Arguments(argc, argv));
  return Game::Run();
}

#undef main
int32_t main(int32_t argc, char** argv) {
  try {
    return (int32_t)Run(argc, argv);
  } catch (const Error& e) {
    FatalError(e);
  } catch (const std::exception& e) {
    FatalError(e);
  }
  return static_cast<int32_t>(RunStatus::Error);
}