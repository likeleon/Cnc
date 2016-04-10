#pragma once

namespace cnc {

class CNC_API File {
public:
  static bool Exists(const std::string& path);
  
  static StreamPtr OpenRead(const std::string& path);  
  static std::vector<char> ReadAllBytes(const std::string& path);
  static std::string ReadAllText(const std::string& path);

  static void WriteAllLines(const std::string& path, const std::vector<std::string>& lines);
};

}
