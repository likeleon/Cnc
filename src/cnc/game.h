#pragma once

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

  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

private:
  static void InitializeSettings(const Arguments& args);

  static std::unique_ptr<Settings> settings_;
  static std::unique_ptr<Renderer> renderer_;
};

}