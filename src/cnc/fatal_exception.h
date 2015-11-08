#pragma once

namespace cnc {

class FatalException : public std::exception {
public:
  explicit FatalException(const std::string& error_message);
  virtual const char* what() const noexcept override;

private:
  std::string error_message_;
};

}