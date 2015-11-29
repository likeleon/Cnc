#include "cnc/stdafx.h"
#include "cnc/debug.h"
#include "cnc/log.h"
#include "cnc/fatal_exception.h"

namespace cnc {

bool Debug::die_on_error_ = false;
bool Debug::abort_on_die_ = false;

void Debug::SetDieOnError(bool die) {
  die_on_error_ = die;
}

void Debug::SetAbortOnDie(bool abort) {
  abort_on_die_ = abort;
}

void Debug::Warning(const std::string& message) {
  Log::Write("debug", "Warning: " + message);
  std::cerr << "Warning: " + message;
}

void Debug::Error(const std::string& message) {
  if (die_on_error_) {
    Die(message);
  }
 
  Log::Write("debug", "Error: " + message);
  std::cerr << "Error: " + message;
}

void Debug::CheckAssertion(bool assertion, const char* error_message) {
  if (!assertion) {
    Die(error_message);
  }
}

void Debug::CheckAssertion(bool assertion, const std::string& error_message) {
  if (!assertion) {
    Die(error_message);
  }
}

void Debug::Die(const std::string& error_message) {
  Log::Write("debug", "Fatal: " + error_message);
  std::cerr << "Fatal: " + error_message;

  if (abort_on_die_) {
    std::abort();
  } else {
    throw FatalException(error_message);
  }
}

}