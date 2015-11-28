#include "cnc/stdafx.h"
#include "cnc/string.h"

namespace cnc {

std::string Trim(const std::string& s, const char* trim_chars) {
  return TrimStart(TrimEnd(s, trim_chars), trim_chars);
}

std::string TrimStart(const std::string& s, const char* trim_chars) {
  std::string str(s);
  if (trim_chars == nullptr) {
    auto end_iter = std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int, int>(std::isspace)));
    str.erase(str.begin(), end_iter);
  } else {
    auto start_trail_index = str.find_first_not_of(trim_chars);
    if (start_trail_index != std::string::npos) {
      str.erase(0, start_trail_index);
    }
  }
  return str;
}

std::string TrimEnd(const std::string& s, const char* trim_chars) {
  std::string str(s);
  if (trim_chars == nullptr) {
    auto end_riter = std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int, int>(std::isspace)));
    str.erase(end_riter.base(), str.end());
  } else {
    auto end_trail_index = str.find_last_not_of(trim_chars);
    if (end_trail_index < str.size() - 1) {
      str.erase(end_trail_index);
    }
  }
  return str;
}

}
