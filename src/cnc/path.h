#pragma once

namespace cnc {

class Path {
public:
  Path() = delete;

  static std::string GetDirectoryName(const std::string& path);
  static std::string GetFileName(const std::string& path);
};

}
