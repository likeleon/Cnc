#pragma once

#include "cnc/stop_watch.h"
#include "cnc/icursor.h"
#include "cnc/iinput_handler.h"

namespace cnc {

class Arguments;
class ModData;
class Settings;
class Renderer;
class ActionQueue;
class OrderManager;
enum class WorldType;

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
  
  static bool IsModInstalled(std::pair<std::string, std::string> mod);
  static bool IsModInstalled(const std::string& mod_id);
  
  static void InitializeMod(const std::string& mod, const Arguments& args);

  static void LoadShellMap();

  static ModData* mod_data();
  static Settings& settings();
  static int64_t RunTime();
  static Renderer* renderer();

  static Modifiers GetModifierKeys();
  static void HandleModifierKeys(Modifiers mods);

  static void RunAfterTick(const std::function<void()>& a);

  Game() = delete;
  Game(const Game&) = delete;
  Game& operator=(const Game&) = delete;

private:
  static const int64_t Timestep = 40;
  
  static void JoinLocal();
  static void JoinInner(std::unique_ptr<OrderManager> om);

  static void InitializeSettings(const Arguments& args);
  static void Loop();
  static void LogicTick();
  static void RenderTick();
  
  static std::string ChooseShellmap();
  static void StartGame(const std::string& map_uid, WorldType type);

  static std::unique_ptr<ModData> mod_data_;
  static std::unique_ptr<Settings> settings_;
  static std::unique_ptr<ICursor> cursor_;
  static std::unique_ptr<OrderManager> order_manager_;
  static std::unique_ptr<Renderer> renderer_;

  static StopWatch stop_watch_;
  static RunStatus state_;
  static int32_t render_frame_;
  
  static Modifiers modifiers_;
  static std::unique_ptr<ActionQueue> delayed_actions_;
};

}