#pragma once

#include "cnc/message.h"

namespace cnc {

class Error : public std::runtime_error {
public:
  Error(const Message& msg, bool store_cause = true);
  
  const char* what() const noexcept override;
  
  virtual std::string type_name() const;
  const Message& message() const;
  
  void RethrowCause() const;

private:
  void StoreCause();
  
  Message message_;
  std::exception_ptr cause_;
};

std::ostream& operator<<(std::ostream& os, const Error& e);

}
