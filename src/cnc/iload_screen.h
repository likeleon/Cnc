#pragma once

namespace cnc {

class Manifest;
class Arguments;

class CNC_API ILoadScreen {
public:
  virtual ~ILoadScreen() {}

  virtual void Init(const Manifest& m, const std::map<std::string, std::string>& info) = 0;
  virtual void Display() = 0;
  virtual void StartGame(const Arguments& args) = 0;
};

}
