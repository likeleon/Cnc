#pragma once

namespace cnc {

class Arguments {
public:
  static const Arguments& Empty();

  Arguments(int argc, char** argv);

  bool Contains(const std::string& key) const;
  std::string GetValue(const std::string& key,
                       const std::string& default_value) const;

private:
  Arguments();

  std::unordered_map<std::string, std::string> args_;
};

}
