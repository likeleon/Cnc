#include "cnc/stdafx.h"
#include "cnc/mini_yaml.h"
#include "cnc/error.h"
#include "cnc/string_utils.h"
#include "cnc/container_utils.h"
#include "cnc/stream.h"
#include "cnc/file.h"

namespace cnc {

MiniYaml::MiniYaml(const std::string& value)
  : MiniYaml(value, nullptr) {
}

MiniYaml::MiniYaml(const std::string& value, MiniYamlNodesPtr nodes)
  : value_(value), nodes_(nodes) {
  if (nodes_ == nullptr) {
    nodes_ = std::make_shared<MiniYamlNodes>();
  }
}

MiniYamlNodes MiniYaml::NodesOrEmpty(const MiniYaml& y, const std::string& s) {
  auto nd = y.ToMap();
  return (nd.find(s) != nd.end()) ? *nd.at(s).nodes_ : MiniYamlNodes();
}

MiniYamlMap MiniYaml::MapFromFile(const std::string& path) {
  MiniYamlMap map;
  auto nodes = FromFile(path);
  std::transform(nodes->begin(), nodes->end(), std::inserter(map, map.end()),
                 [](const auto& node) { return std::make_pair(node.key(), node.value()); });
  return map;
}

static std::vector<std::string> ReadAllLines(const std::string& path) {
  std::ifstream file(path);
  if (!file) {
    throw Error(MSG("Cannot open file: " + path));
  }

  std::vector<std::string> lines;
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
    key = StringUtils::Trim(t.substr(0, colon));
    value = StringUtils::Trim(t.substr(colon + 1));
  } else {
    key = t;
  }
  return std::make_pair(key, value);
}

static MiniYamlNodesPtr FromLines(const std::vector<std::string>& lines, const std::string& filename) {
  std::vector<MiniYamlNodesPtr> levels;
  levels.push_back(std::make_shared<MiniYamlNodes>());

  int32_t line_no = 0;
  for (const auto& l : lines) {
    ++line_no;

    std::string line(l);
    size_t comment_index = line.find('#');
    if (comment_index != std::string::npos) {
      line = StringUtils::TrimEnd(line.substr(0, comment_index), " \t");
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

    MiniYamlNodesPtr nodes(std::make_shared<MiniYamlNodes>());
    auto pair = SplitAtColon(t);
    levels[level]->emplace_back(pair.first, pair.second, nodes, location);

    levels.push_back(nodes);
  }

  return *levels.begin();
}

MiniYamlNodesPtr MiniYaml::FromFile(const std::string& path) {
  return FromLines(ReadAllLines(path), path);
}

MiniYamlNodesPtr MiniYaml::FromStream(StreamPtr s, const std::string& file_name) {
  auto bytes = s->ReadBytes(s->Length());
  return FromString(std::string(bytes.data(), bytes.size()), file_name);
}

MiniYamlNodesPtr MiniYaml::FromString(const std::string& text, const std::string& file_name) {
  return FromLines(StringUtils::Split(text, '\n', StringSplitOptions::RemoveEmptyEntries), file_name);
}

std::vector<MiniYamlNodes> MiniYaml::FromFiles(const std::vector<std::string>& paths) {
  std::vector<MiniYamlNodes> yy;
  for (const auto& f : paths) {
    yy.emplace_back(*MiniYaml::FromFile(f));
  }
  return yy;
}

static std::string TrimEndWithNullTrimChars(const std::string& s) {
  return StringUtils::TrimEnd(s);
}

void MiniYaml::WriteToFile(const MiniYamlNodes& y, const std::string& filename) {
  auto lines = ToLines(y, true);
  std::transform(lines.begin(), lines.end(), lines.begin(), TrimEndWithNullTrimChars);
  File::WriteAllLines(filename, lines);
}

std::string MiniYaml::WriteToString(const MiniYamlNodes& y) {
  auto lines = ToLines(y, true);
  std::transform(lines.begin(), lines.end(), lines.begin(), TrimEndWithNullTrimChars);
  return StringUtils::Join(lines, "\n");
}

std::vector<std::string> MiniYaml::ToLines(const MiniYamlNodes& y, bool lowest) {
  std::vector<std::string> ret;
  for (const auto& kv : y) {
    for (const auto& line : kv.value().ToLines(kv.key())) {
      ret.emplace_back(line);
    }
    if (lowest) {
      ret.emplace_back("");
    }
  }
  return ret;
}


MiniYamlNodes MiniYaml::MergePartial(const MiniYamlNodes& a, const MiniYamlNodes& b) {
  if (a.empty()) {
    return b;
  }

  if (b.empty()) {
    return a;
  }

  MiniYamlNodes nodes;
  
  auto key_selector = [](const MiniYamlNode& n) { return n.key(); };
  auto log_value = [](const MiniYamlNode& n) { 
    std::ostringstream oss;
    oss << n.key() << "( at " << n.location().ToString() << ")";
    return oss.str();
  };
  auto map_a = ToMapWithConflictLog<MiniYamlNode, std::string>(a, key_selector, "MiniYaml::Merge", nullptr, log_value);
  auto map_b = ToMapWithConflictLog<MiniYamlNode, std::string>(b, key_selector, "MiniYaml::Merge", nullptr, log_value);
  
  std::set<std::string> all_keys;
  for (const auto& kvp : map_a) {
    all_keys.insert(kvp.first);
  }
  for (const auto& kvp : map_b) {
    all_keys.insert(kvp.first);
  }

  for (const auto& key : all_keys) {
    auto aa = map_a.find(key);
    auto bb = map_b.find(key);
    
    if (aa == map_a.end()) {
      nodes.emplace_back(bb->second);
    } else if (bb == map_b.end()) {
      nodes.emplace_back(aa->second);
    } else {
      auto loc = (aa == map_a.end()) ? MiniYamlNode::SourceLocation() : aa->second.location();
      nodes.emplace_back(key, MergePartial(aa->second.value(), bb->second.value()), loc);
    }
  }
  
  return nodes;
}

MiniYaml MiniYaml::MergePartial(const MiniYaml& a, const MiniYaml& b) {
  std::string key = a.value();
  if (key.empty()) {
    key = b.value();
  }
  auto nodes = std::make_shared<MiniYamlNodes>(MergePartial(a.nodes(), b.nodes()));
  return{ key, nodes };
}

MiniYamlNodes MiniYaml::ApplyRemovals(const MiniYamlNodes& a) {
  std::set<std::string> remove_keys;
  for (const auto& node : a) {
    auto key = node.key();
    if (key.empty() || key[0] != '-') {
      continue;
    }
    remove_keys.insert(key.substr(1));
  }

  MiniYamlNodes nodes;
  for (const auto& x : a) {
    if (x.key()[0] == '-') {
      continue;
    }

    if (remove_keys.find(x.key()) != remove_keys.end()) {
      remove_keys.erase(x.key());
    } else {
      nodes.emplace_back(
        x.key(),
        x.value().value(),
        std::make_shared<MiniYamlNodes>(ApplyRemovals(x.value().nodes())));
    }
  }

  if (!remove_keys.empty()) {
    std::ostringstream oss;
    oss << "Bogus yaml removals: " << StringUtils::Join({ remove_keys.begin(), remove_keys.end() }, ", ");
    throw Error(MSG(oss.str()));
  }

  return nodes;
}

static MiniYaml MiniYamlIdentity(const MiniYaml& y) {
  return y;
};

MiniYamlMap MiniYaml::ToMap() const {
  return ToMap<MiniYaml>(MiniYamlIdentity);
}

std::vector<std::string> MiniYaml::ToLines(const std::string& name) const {
  std::vector<std::string> ret;
  ret.emplace_back(name + ": " + value_);
  if (nodes_ != nullptr) {
    for (const auto& line : ToLines(*nodes_, false)) {
      ret.emplace_back("\t" + line);
    }
  }
  return ret;
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