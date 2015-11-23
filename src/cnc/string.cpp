#include "cnc/stdafx.h"
#include "cnc/string.h"

namespace cnc {

std::wstring Trim(const std::wstring& s, const wchar_t* trim_chars) {
  return TrimStart(TrimEnd(s, trim_chars), trim_chars);
}

std::wstring TrimStart(const std::wstring& s, const wchar_t* trim_chars) {
  std::wstring str(s);
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

std::wstring TrimEnd(const std::wstring& s, const wchar_t* trim_chars) {
  std::wstring str(s);
  if (trim_chars == nullptr) {
    auto end_riter = std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int, int>(std::isspace)));
    str.erase(end_riter.base(), s.end());
  } else {
    auto end_trail_index = str.find_last_not_of(trim_chars);
    if (end_trail_index < str.size() - 1) {
      str.erase(end_trail_index);
    }
  }
  return str;
}

}
