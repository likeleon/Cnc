#include "cnc/stdafx.h"
#include "cnc/string_utils.h"

namespace cnc {

static bool CharCompareIgnoreCase(char a, char b) {
  return (toupper(a) == toupper(b));
}

bool StringUtils::Equals(const std::string& a, const std::string& b, bool ignore_case) {
  if (!ignore_case) {
    return a == b;
  } else {
    if (a.size() != b.size()) {
      return false;
    }
    return std::equal(a.begin(), a.end(), b.begin(), CharCompareIgnoreCase);
  }
}

bool StringUtils::Contains(const std::string& s, const std::string& value) {
  return s.find(value) != std::string::npos;
}

bool StringUtils::StartsWith(const std::string& s, const std::string& starting) {
  return (s.find(starting) == 0);
}

bool StringUtils::EndsWith(const std::string& s, const std::string& ending) {
  if (s.length() < ending.length()) {
    return false;
  }
  return (s.compare(s.length() - ending.length(), ending.length(), ending) == 0);
}

std::string StringUtils::Replace(const std::string& s, const std::string& old_value, const std::string& new_value) {
  std::string ret = s;
  size_t start_pos = 0;
  while ((start_pos = ret.find(old_value, start_pos)) != std::string::npos) {
    ret.replace(start_pos, old_value.length(), new_value);
    start_pos += new_value.length();
  }
  return ret;
}

std::string StringUtils::Trim(const std::string& s, const char* trim_chars) {
  return TrimStart(TrimEnd(s, trim_chars), trim_chars);
}

static int32_t IsSpace(char c) {
  return std::isspace(static_cast<unsigned char>(c));
}

std::string StringUtils::TrimStart(const std::string& s, const char* trim_chars) {
  std::string str(s);
  if (trim_chars == nullptr) {
    auto end_iter = std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun(IsSpace)));
    str.erase(str.begin(), end_iter);
  } else {
    auto start_trail_index = str.find_first_not_of(trim_chars);
    if (start_trail_index != std::string::npos) {
      str.erase(0, start_trail_index);
    }
  }
  return str;
}

std::string StringUtils::TrimEnd(const std::string& s, const char* trim_chars) {
  std::string str(s);
  if (trim_chars == nullptr) {
    auto end_riter = std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun(IsSpace)));
    str.erase(end_riter.base(), str.end());
  } else {
    auto end_trail_index = str.find_last_not_of(trim_chars);
    if (end_trail_index < str.size() - 1) {
      str.erase(end_trail_index);
    }
  }
  return str;
}

std::string StringUtils::PadRight(const std::string& s, int32_t total_width, char padding_char) {
  std::string str(s);
  str.append(total_width - str.length(), padding_char);
  return str;
}

std::string StringUtils::ToLower(const std::string& s) {
  std::string str(s);
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
}

std::string StringUtils::ToUpper(const std::string& s) {
  std::string str(s);
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  return str;
}

std::string StringUtils::Join(const std::vector<std::string>& values, const std::string& separator) {
  std::string str;
  for (auto iter = values.begin(); iter != values.end(); ++iter) {
    str += *iter;
    if (iter != values.end() - 1) {
      str += separator;
    }
  }
  return str;
}

std::vector<std::string> StringUtils::Split(const std::string& s, char delimiter, StringSplitOptions options) {
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
