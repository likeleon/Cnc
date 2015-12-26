#pragma once

#include "cnc/stop_watch.h"

namespace cnc {

class Arguments;
class ModData;
class Settings;
class Renderer;

enum class RunStatus {
  Error = -1,
  Success = 0,
  Running = 1
};

class CNC_API Game {
public:
  static void Initialize(const Arguments& args);
  
  static RunStatus Run();
  static void Exit();

  static const ModData* mod_data();
  static Settings& settings();
  static int64_t RunTime();
  static Renderer* renderer();

  Game() = delete;
  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

private:
  static void InitializeSettings(const Arguments& args);
  static void InitializeMod(const std::string& mod, const Arguments& args);
  static void Loop();
  static void RenderTick();

  static const int64_t Timestep = 40;

  static std::unique_ptr<ModData> mod_data_;
  static std::unique_ptr<Settings> settings_;
  static std::unique_ptr<Renderer> renderer_;

  static StopWatch stop_watch_;
  static RunStatus state_;
  static int32_t render_frame_;
};

}