#pragma once

namespace cnc {

std::wstring Trim(const std::wstring& s, const wchar_t* trim_chars = nullptr);
std::wstring TrimStart(const std::wstring& s, const wchar_t* trim_chars = nullptr);
std::wstring TrimEnd(const std::wstring& s, const wchar_t* trim_chars = nullptr);

}