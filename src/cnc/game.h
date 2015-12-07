#pragma once

#include "cnc/stop_watch.h"

namespace cnc {

class Arguments;
class Settings;
class Renderer;

enum class RunStatus {
  Error = -1,
  Success = 0,
  Running = std::numeric_limits<int32_t>::max()
};

class Game {
public:
  static void Initialize(const Arguments& args);
  static RunStatus Run();

  static Settings& settings() { return *settings_; }

  static int64_t RunTime();

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

private:
  static void InitializeSettings(const Arguments& args);
  static void Loop();

  static const int64_t Timestep = 40;

  static std::unique_ptr<Settings> settings_;
  static std::unique_ptr<Renderer> renderer_;

  static StopWatch stop_watch_;
  static RunStatus state_;
};

}