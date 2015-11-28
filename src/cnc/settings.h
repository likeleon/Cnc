#pragma once

namespace cnc {

class Arguments;

class Settings {
public:
  Settings(const std::string& path, const Arguments& args);

private:
  std::string settings_file_;
};

}
