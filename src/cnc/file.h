#pragma once

namespace cnc {

class CNC_API File {
public:
  static bool Exists(const std::string& path);
  
  static std::vector<char> OpenRead(const std::string& path);
  static std::string ReadAllText(const std::string& path);
};

}
