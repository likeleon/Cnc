#pragma once

namespace cnc {

class YamlException : public std::exception {
public:
  YamlException(const std::string& error_message);

  const char* what() const throw () override;

private:
  std::string error_message_;
};

}