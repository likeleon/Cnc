#pragma once

namespace cnc {

class CNC_API Arguments {
public:
  Arguments(int32_t argc, char** argv);

  bool Contains(const std::string& key) const;
  std::string GetValue(const std::string& key,
                       const std::string& default_value) const;

  static const Arguments Empty;

private:
  Arguments();

  std::unordered_map<std::string, std::string> args_;
};

}
