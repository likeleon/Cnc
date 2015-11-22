#include "cnc/stdafx.h"
#include "cnc/string.h"

namespace cnc {

std::wstring TrimEnd(const std::wstring& s, const wchar_t* trim_chars) {
  std::wstring str(s);
  if (trim_chars == nullptr) {
    str.erase(std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  } else {
    auto end_trail_index = str.find_last_not_of(trim_chars);
    if (end_trail_index < str.size() - 1) {
      str.erase(end_trail_index);
    }
  }
  return str;
}

}
