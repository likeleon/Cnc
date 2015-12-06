#pragma once

namespace cnc {

enum class StringSplitOptions {
  None,
  RemoveEmptyEntries
};

class String {
public:
  static bool StartsWith(const std::string& s, const std::string& starting);
  static bool EndsWith(const std::string& s, const std::string& ending);

  static std::string Trim(const std::string& s, const char* trim_chars = nullptr);
  static std::string TrimStart(const std::string& s, const char* trim_chars = nullptr);
  static std::string TrimEnd(const std::string& s, const char* trim_chars = nullptr);

  static std::string PadRight(const std::string&s, int32_t total_width, char padding_char);

  static std::string ToUpper(const std::string& s);

  static std::string Join(const std::vector<std::string>& values, const std::string& separator);

  static std::vector<std::string> Split(const std::string& s, char delimiter, StringSplitOptions options = StringSplitOptions::None);
};

}