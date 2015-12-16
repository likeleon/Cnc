#include "cnc/stdafx.h"
#include "cnc/mini_yaml.h"
#include "cnc/error.h"
#include "cnc/string.h"

namespace cnc {

MiniYaml::MiniYaml(const std::string& value)
  : MiniYaml(value, nullptr) {
}

MiniYaml::MiniYaml(const std::string& value, MiniYamlNodesPtr nodes)
  : value_(value), nodes_(nodes) {
  if (nodes_ == nullptr) {
    nodes_ = std::make_shared<std::vector<MiniYamlNode>>();
  }
}

std::unordered_map<std::string, MiniYaml> MiniYaml::MapFromFile(const std::string& path) {
  std::unordered_map<std::string, MiniYaml> map;
  auto nodes = FromFile(path);
  std::transform(nodes->begin(), nodes->end(), std::inserter(map, map.end()),
                 [](const auto& node) { return std::make_pair(node.key(), node.value()); });
  return map;
}

static std::list<std::string> ReadAllLines(const std::string& path) {
  std::ifstream file(path);
  if (!file) {
    throw Error(MSG("Cannot open file: " + path));
  }

  std::list<std::string> lines;
  std::string line;
  while (std::getline(file, line)) {
    lines.emplace_back(line);
  }
  return lines;
}

static std::pair<std::string, std::string> SplitAtColon(const std::string& t) {
  std::string key;
  std::string value;
  size_t colon = t.find(':');
  if (colon != -1) {
    key = String::Trim(t.substr(0, colon));
    value = String::Trim(t.substr(colon + 1));
  } else {
    key = t;
  }
  return std::make_pair(key, value);
}

static MiniYamlNodesPtr FromLines(const std::list<std::string>& lines, const std::string& filename) {
  std::vector<MiniYamlNodesPtr> levels;
  levels.push_back(std::make_shared<std::vector<MiniYamlNode>>());

  int32_t line_no = 0;
  for (const auto& l : lines) {
    ++line_no;

    std::string line(l);
    size_t comment_index = line.find('#');
    if (comment_index != -1) {
      String::TrimEnd(line.substr(0, comment_index), " \t");
    }

    if (line.empty()) {
      continue;
    }

    const int32_t SpacesPerLevel = 4;
    size_t cp = 0;
    size_t level = 0;
    int32_t spaces = 0;
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

    std::string t = line.substr(cp);
    if (t.empty()) {
      continue;
    }
    MiniYamlNode::SourceLocation location(filename, line_no);

    if (levels.size() <= level) {
      throw Error(MSG("Bad indent in miniyaml at " + location.ToString()));
    }
    while (levels.size() > level + 1) {
      levels.erase(levels.begin() + static_cast<int32_t>(levels.size()) - 1);
    }

    MiniYamlNodesPtr nodes(std::make_shared<std::vector<MiniYamlNode>>());
    auto pair = SplitAtColon(t);
    levels[level]->emplace_back(pair.first, pair.second, nodes, location);

    levels.push_back(nodes);
  }

  return *levels.begin();
}

MiniYamlNodesPtr MiniYaml::FromFile(const std::string& path) {
  return FromLines(ReadAllLines(path), path);
}

std::unordered_map<std::string, MiniYaml> MiniYaml::ToMap() const {
  std::unordered_map<std::string, MiniYaml> result;
  for (const auto& node : nodes()) {
    bool added = result.emplace(node.key(), node.value()).second;
    if (!added) {
      throw Error(MSG("Duplicate key '" + node.key() + "' in " + node.location().ToString()));
    }
  }
  return result;
}

MiniYamlNode::SourceLocation::SourceLocation() {
}

MiniYamlNode::SourceLocation::SourceLocation(const std::string& filename, int32_t line)
  : filename_(filename), line_(line) {
}

std::string MiniYamlNode::SourceLocation::ToString() const {
  std::ostringstream oss;
  oss << filename() << ":" << line();
  return oss.str();
}

MiniYamlNode::MiniYamlNode(const std::string& k, MiniYaml&& v)
  : key_(k), value_(std::move(v)) {
}

MiniYamlNode::MiniYamlNode(const std::string& k, MiniYaml&& v, const SourceLocation& loc)
  : MiniYamlNode(k, std::move(v)) {
  location_ = loc;
}

MiniYamlNode::MiniYamlNode(const std::string& k, const std::string& v)
  : MiniYamlNode(k, v, nullptr) {
}

MiniYamlNode::MiniYamlNode(const std::string& k, const std::string& v, MiniYamlNodesPtr n)
  : MiniYamlNode(k, MiniYaml(v, n)) {
}

MiniYamlNode::MiniYamlNode(const std::string& k, const std::string& v, MiniYamlNodesPtr n, const SourceLocation& loc)
  : MiniYamlNode(k, MiniYaml(v, n), loc) {
}

std::string MiniYamlNode::ToString() const {
  std::ostringstream oss;
  oss << "{{YamlNode: {" << key_ << "} @ {" + location_.ToString() << "}}}";
  return oss.str();
}

}