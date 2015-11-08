#include "cnc/stdafx.h"
#include "cnc/arguments.h"

namespace cnc {

const Arguments& Arguments::Empty() {
  static Arguments args;
  return args;
}

Arguments::Arguments() {
}

Arguments::Arguments(int argc, char** argv) {
  std::regex regex("([^=]+)=(.*)");
  for (int i = 0; i < argc; ++i) {
    std::cmatch match;
    bool success = std::regex_match(argv[i], match, regex);
    if (!success || match.size() < 3) {
      continue;
    }

    args_.emplace(match[1], match[2]);
  }
}

bool Arguments::Contains(const std::string& key) const {
  return args_.find(key) != args_.end();
}

std::string Arguments::GetValue(const std::string& key,
                                const std::string& default_value) const {
  auto it = args_.find(key);
  return (it != args_.end()) ? it->second : default_value;
}

}