#pragma once

namespace cnc {

class Arguments;

class Settings {
public:
  Settings(const Path& path, const Arguments& args);

private:
  Path settings_file_;
};

}
