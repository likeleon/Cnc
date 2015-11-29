#pragma once

namespace cnc {

std::string Trim(const std::string& s, const char* trim_chars = nullptr);
std::string TrimStart(const std::string& s, const char* trim_chars = nullptr);
std::string TrimEnd(const std::string& s, const char* trim_chars = nullptr);

std::string Join(const std::vector<std::string>& values, const std::string& separator);

}