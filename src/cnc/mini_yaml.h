#pragma once

namespace cnc {

class MiniYamlNode;

using MiniYamlNodesPtr = std::shared_ptr<std::vector<MiniYamlNode>>;

class MiniYaml {
public:
  explicit MiniYaml(const std::string& value);
  MiniYaml(const std::string& value, MiniYamlNodesPtr nodes);

  static std::unordered_map<std::string, MiniYaml> MapFromFile(const std::string& path);
  static MiniYamlNodesPtr FromFile(const std::string& path);

  std::unordered_map<std::string, MiniYaml> ToMap() const;

  const std::string& value() const { return value_; }
  const std::vector<MiniYamlNode>& nodes() const { return *nodes_; }

private:
  std::string value_;
  MiniYamlNodesPtr nodes_;
};

class MiniYamlNode {
public:
  class SourceLocation {
  public:
    SourceLocation();
    SourceLocation(const std::string& filename, int line);

    const std::string& filename() const { return filename_; }
    int line() const { return line_; }
    std::string ToString() const;

  private:
    std::string filename_;
    int line_ = 0;
  };

  MiniYamlNode(const std::string& k, MiniYaml&& v);
  MiniYamlNode(const std::string& k, MiniYaml&& v, const SourceLocation& loc);
  MiniYamlNode(const std::string& k, const std::string& v);
  MiniYamlNode(const std::string& k, const std::string& v, MiniYamlNodesPtr n);
  MiniYamlNode(const std::string& k, const std::string& v, MiniYamlNodesPtr n, const SourceLocation& loc);

  const std::string& key() const { return key_; }
  const MiniYaml& value() const { return value_; }

  std::string ToString() const;

private:
  std::string key_;
  MiniYaml value_;
  SourceLocation location_;
};

}
