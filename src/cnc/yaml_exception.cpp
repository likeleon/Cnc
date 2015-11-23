#include "cnc/stdafx.h"
#include "cnc/yaml_exception.h"

namespace cnc {

YamlException::YamlException(const std::string& error_message)
  : error_message_(error_message) {
}

const char* YamlException::what() const throw () {
  return error_message_.c_str();
}

}