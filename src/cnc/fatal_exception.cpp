#include "cnc/stdafx.h"
#include "cnc/fatal_exception.h"

namespace cnc {

FatalException::FatalException(const std::string& error_message)
  : error_message_(error_message) {
}

const char* FatalException::what() const noexcept {
  return error_message_.c_str();
}

}