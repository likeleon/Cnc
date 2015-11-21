#include "cnc/stdafx.h"
#include "cnc/mini_yaml.h"

namespace cnc {

std::string MiniYamlNode::SourceLocation::ToString() const {
  std::ostringstream oss;
  oss << filename << ":" << line;
  return oss.str();
}

std::unordered_map<std::string, MiniYaml> MiniYaml::MapFromFile(const Path& path) {
  std::unordered_map<std::string, MiniYaml> map;
  auto list = FromFile(path);
  std::transform(list.begin(), list.end(), std::inserter(map, map.end()), 
                 [](const auto& node) { return std::make_pair(node.key, node.value); });
  return map;
}

static std::list<std::string> ReadAllLines(const Path& path) {
  std::ifstream file(path);
  if (!file) {
    // TODO: Debug::error()
  }

  std::list<std::string> lines;
  std::string line;
  while (std::getline(file, line)) {
    lines.emplace_back(line);
  }
  return lines;
}

static std::list<MiniYamlNode> FromLines(const std::list<std::string>& lines, const Path& filename) {
  std::list<std::list<MiniYamlNode>> levels;
  levels.emplace_back();

  int line_no = 0;
  for (const auto& line : lines) {
    ++line_no;
    
    // TODO
  }

  return *levels.begin();
}

std::list<MiniYamlNode> MiniYaml::FromFile(const Path& path) {
  return FromLines(ReadAllLines(path), path);
}

}