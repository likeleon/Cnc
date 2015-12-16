#include "cnc/stdafx.h"
#include "cnc/error.h"

namespace cnc {

Error::Error(const Message& msg, bool store_cause)
  : std::runtime_error("Cnc error object"), message_(msg) {
  if (store_cause) {
    StoreCause();
  }
}

const char* Error::what() const noexcept {
  return message_.text.c_str();
}

void Error::StoreCause() {
  if (!std::current_exception()) {
    return;
  }

  try {
    throw;
  } catch (...) {
    cause_ = std::current_exception();
  }
}

void Error::RethrowCause() const {
  if (cause_) {
    std::rethrow_exception(cause_);
  }
}

std::string Error::type_name() const {
  return typeid(*this).name();
}

const Message& Error::message() const {
  return message_;
}

struct BacktraceSymbol {
  std::string file_name;
  uint32_t line_no;
  std::string function_name;
  void* pc;
};


static std::ostream &operator <<(std::ostream &os, const BacktraceSymbol &bt_sym) {
  os << "  File ";

  if (bt_sym.file_name.empty()) {
    os << '?';
  } else {
    os << '"' << bt_sym.file_name << '"';
  }

  if (bt_sym.line_no) {
    os << ", line " << bt_sym.line_no;
  }

  os << ", in ";

  if (bt_sym.function_name.empty()) {
    os << '?';
  } else {
    os << bt_sym.function_name;
  }

  if (bt_sym.pc != nullptr) {
    os << " [" << bt_sym.pc << "]";
  }

  return os;
}

std::ostream& operator<<(std::ostream& os, const Error& e) {
  bool had_a_cause = true;
  try {
    e.RethrowCause();
    had_a_cause = false;
  } catch (Error& cause) {
    os << cause << std::endl;
  } catch (std::exception& cause) {
    os << typeid(cause).name() << ": " << cause.what() << std::endl;
  }

  if (had_a_cause) {
    os << std::endl << "The above exception was the direct cause of the following exception:" << std::endl << std::endl;
  }

  os << "origin:" << std::endl;
  
  os << BacktraceSymbol{ 
    e.message().file_name, 
    e.message().line_no, 
    e.message().function_name, 
    nullptr } << std::endl;

  os << e.type_name();
  
  if (!e.message().text.empty()) {
    os << ": " << e.message().text;
  }

  return os;
}

}