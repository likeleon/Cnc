#include "cnc/stdafx.h"
#include "cnc/string.h"

namespace cnc {

bool String::Contains(const std::string& s, const std::string& value) {
  return s.find(value) != std::string::npos;
}

bool String::StartsWith(const std::string& s, const std::string& starting) {
  return (s.find(starting) == 0);
}

bool String::EndsWith(const std::string& s, const std::string& ending) {
  if (s.length() < ending.length()) {
    return false;
  }
  return (s.compare(s.length() - ending.length(), ending.length(), ending) == 0);
}

std::string String::Trim(const std::string& s, const char* trim_chars) {
  return TrimStart(TrimEnd(s, trim_chars), trim_chars);
}

std::string String::TrimStart(const std::string& s, const char* trim_chars) {
  std::string str(s);
  if (trim_chars == nullptr) {
    auto end_iter = std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int32_t, int32_t>(std::isspace)));
    str.erase(str.begin(), end_iter);
  } else {
    auto start_trail_index = str.find_first_not_of(trim_chars);
    if (start_trail_index != std::string::npos) {
      str.erase(0, start_trail_index);
    }
  }
  return str;
}

std::string String::TrimEnd(const std::string& s, const char* trim_chars) {
  std::string str(s);
  if (trim_chars == nullptr) {
    auto end_riter = std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int32_t, int32_t>(std::isspace)));
    str.erase(end_riter.base(), str.end());
  } else {
    auto end_trail_index = str.find_last_not_of(trim_chars);
    if (end_trail_index < str.size() - 1) {
      str.erase(end_trail_index);
    }
  }
  return str;
}

std::string String::PadRight(const std::string& s, int32_t total_width, char padding_char) {
  std::string str(s);
  str.append(total_width - str.length(), padding_char);
  return str;
}

std::string String::ToUpper(const std::string& s) {
  std::string str(s);
  std::transform(str.begin(), str.end(), str.begin(), toupper);
  return str;
}

std::string String::Join(const std::vector<std::string>& values, const std::string& separator) {
  std::string str;
  for (auto iter = values.begin(); iter != values.end(); ++iter) {
    str += *iter;
    if (iter != values.end() - 1) {
      str += separator;
    }
  }
  return str;
}

std::vector<std::string> String::Split(const std::string& s, char delimiter, StringSplitOptions options) {
  std::vector<std::string> result;
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delimiter)) {
    if (options == StringSplitOptions::RemoveEmptyEntries && item.empty()) {
      continue;
    }
    result.emplace_back(item);
  }
  return result;
}

}
