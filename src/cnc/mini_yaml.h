#pragma once

#include <functional>
#include <string>
#include "cnc/message.h"

namespace cnc {

class MiniYamlNode;
using MiniYamlNodesPtr = std::shared_ptr<std::vector<MiniYamlNode>>;

class MiniYaml;
using MiniYamlMap = std::map<std::string, MiniYaml>;

class CNC_API MiniYaml {
public:
  explicit MiniYaml(const std::string& value);
  MiniYaml(const std::string& value, MiniYamlNodesPtr nodes);

  static MiniYamlMap MapFromFile(const std::string& path);
  static MiniYamlNodesPtr FromFile(const std::string& path);

  MiniYamlMap ToMap() const;
  
  template <typename T>
  using ElementSelector = std::function<T (const MiniYaml& y)>;

  template <typename T>
  std::map<std::string, T> ToMap(const ElementSelector<T>& element_selector) const {
    std::map<std::string, T> map;
    for (const auto& node : nodes()) {
      T element = element_selector(node.value());
      bool added = map.emplace(node.key(), element).second;
      if (!added) {
        throw Error(MSG("Duplicate key '" + node.key() + "' in " + node.location().ToString()));
      }
    }
    return map;
  }

  const std::string& value() const { return value_; }
  const std::vector<MiniYamlNode>& nodes() const { return *nodes_; }

private:
  std::string value_;
  MiniYamlNodesPtr nodes_;
};

class CNC_API MiniYamlNode {
public:
  class SourceLocation {
  public:
    SourceLocation();
    SourceLocation(const std::string& filename, int32_t line);

    const std::string& filename() const { return filename_; }
    int32_t line() const { return line_; }
    std::string ToString() const;

  private:
    std::string filename_;
    int32_t line_ = 0;
  };

  MiniYamlNode(const std::string& k, MiniYaml&& v);
  MiniYamlNode(const std::string& k, MiniYaml&& v, const SourceLocation& loc);
  MiniYamlNode(const std::string& k, const std::string& v);
  MiniYamlNode(const std::string& k, const std::string& v, MiniYamlNodesPtr n);
  MiniYamlNode(const std::string& k, const std::string& v, MiniYamlNodesPtr n, const SourceLocation& loc);

  const std::string& key() const { return key_; }
  const MiniYaml& value() const { return value_; }
  const SourceLocation& location() const { return location_; }

  std::string ToString() const;

private:
  std::string key_;
  MiniYaml value_;
  SourceLocation location_;
};

}
