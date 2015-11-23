#pragma once

namespace cnc {

struct MiniYamlNode;

using MiniYamlNodesPtr = std::shared_ptr<std::vector<MiniYamlNode>>;

class MiniYaml {
public:
  static std::unordered_map<std::string, MiniYaml> MapFromFile(const Path& path);
  static MiniYamlNodesPtr FromFile(const Path& path);
};

class MiniYamlNode {
public:
  class SourceLocation {
  public:
    SourceLocation(const Path& filename, int line);

    const Path& filename() const { return filename_; }
    int line() const { return line_; }
    std::string ToString() const;

  private:
    Path filename_;
    int line_;
  };

  MiniYamlNode(const std::wstring& k, const MiniYaml& v);
  MiniYamlNode(const std::wstring& k, const MiniYaml& v, const SourceLocation& loc);
  MiniYamlNode(const std::wstring& k, const std::wstring& v);
  MiniYamlNode(const std::wstring& k, const std::wstring& v, MiniYamlNodesPtr n, const SourceLocation& loc);

  std::wstring ToString() const;

private:
  std::string key_;
  MiniYaml value_;
};


}
