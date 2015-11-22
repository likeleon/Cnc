#include "cnc/stdafx.h"
#include "cnc/mini_yaml.h"
#include "cnc/string.h"

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

static std::list<std::wstring> ReadAllLines(const Path& path) {
  std::wifstream file(path);
  if (!file) {
    // TODO: Debug::error()
  }

  std::list<std::wstring> lines;
  std::wstring line;
  while (std::getline(file, line)) {
    lines.emplace_back(line);
  }
  return lines;
}

static std::list<MiniYamlNode> FromLines(const std::list<std::wstring>& lines, const Path& /*filename*/) {
  std::list<std::list<MiniYamlNode>> levels;
  levels.emplace_back();

  int line_no = 0;
  for (const auto& l : lines) {
    ++line_no;
    
    std::wstring line(l);
    size_t comment_index = line.find(L'#');
    if (comment_index != -1)
      TrimEnd(line.substr(0, comment_index), L" \t");
    if (line.empty())
      continue;

    // TODO
  }

  return *levels.begin();
}

std::list<MiniYamlNode> MiniYaml::FromFile(const Path& path) {
  return FromLines(ReadAllLines(path), path);
}

}