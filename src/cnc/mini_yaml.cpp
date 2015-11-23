#include "cnc/stdafx.h"
#include "cnc/mini_yaml.h"
#include "cnc/string.h"
#include "cnc/yaml_exception.h"

namespace cnc {

MiniYamlNode::SourceLocation::SourceLocation(const Path& filename, int line)
  : filename_(filename), line_(line) {
}

std::string MiniYamlNode::SourceLocation::ToString() const {
  std::ostringstream oss;
  oss << filename() << ":" << line();
  return oss.str();
}

std::unordered_map<std::string, MiniYaml> MiniYaml::MapFromFile(const Path& path) {
  std::unordered_map<std::string, MiniYaml> map;
  auto nodes = FromFile(path);
  std::transform(nodes->begin(), nodes->end(), std::inserter(map, map.end()),
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

static std::pair<std::wstring, std::wstring> SplitAtColon(const std::wstring& t, 
                                                          const MiniYamlNode::SourceLocation& location) {
  size_t colon = t.find(L':');
  if (colon == -1) {
    throw YamlException("Colon not found at " + location.ToString());
  }

  std::wstring value = Trim(t.substr(colon + 1));
  std::wstring key = Trim(t.substr(0, colon));
  return std::make_pair(key, value);
}

static MiniYamlNodesPtr FromLines(const std::list<std::wstring>& lines, const Path& filename) {
  std::vector<MiniYamlNodesPtr> levels;
  levels.push_back(std::make_shared<std::vector<MiniYamlNode>>());

  int line_no = 0;
  for (const auto& l : lines) {
    ++line_no;

    std::wstring line(l);
    size_t comment_index = line.find(L'#');
    if (comment_index != -1) {
      TrimEnd(line.substr(0, comment_index), L" \t");
    }

    if (line.empty()) {
      continue;
    }

    const int SpacesPerLevel = 4;
    size_t cp = 0;
    size_t level = 0;
    int spaces = 0;
    wchar_t c = line[cp];
    bool text_start = false;
    while (!(c == L'\n' || c == L'\r') && cp < line.length() && !text_start) {
      c = line[cp];
      switch (c) {
      case L' ':
        ++spaces;
        if (spaces >= SpacesPerLevel) {
          spaces = 0;
          ++level;
        }

        ++cp;
        break;
      case L'\t':
        ++level;
        ++cp;
        break;
      default:
        text_start = true;
        break;
      }
    }

    std::wstring t = line.substr(cp);
    if (t.empty()) {
      continue;
    }
    MiniYamlNode::SourceLocation location(filename, line_no);

    if (levels.size() <= level) {
      throw YamlException("Bad indent in miniyaml at " + location.ToString());
    }
    while (levels.size() > level + 1) {
      levels.erase(levels.begin() + static_cast<int>(levels.size()) - 1);
    }

    MiniYamlNodesPtr nodes(std::make_shared<std::vector<MiniYamlNode>>());
    auto pair = SplitAtColon(t, location);
    levels[level]->emplace_back(pair.first, pair.second, nodes, location);
    
    levels.push_back(nodes);
  }

  return *levels.begin();
}

MiniYamlNodesPtr MiniYaml::FromFile(const Path& path) {
  return FromLines(ReadAllLines(path), path);
}

}