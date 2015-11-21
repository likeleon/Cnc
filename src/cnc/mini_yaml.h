#pragma once

namespace cnc {

struct MiniYamlNode;

class MiniYaml {
public:
  static std::unordered_map<std::string, MiniYaml> MapFromFile(const Path& path);
  static std::list<MiniYamlNode> FromFile(const Path& path);
};

struct MiniYamlNode {
  struct SourceLocation {
    Path filename;
    int line;

    std::string ToString() const;
  };
  std::string key;
  MiniYaml value;
};


}
