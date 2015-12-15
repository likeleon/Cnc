#pragma once

#include <string>

#ifndef NDEBUG
#define CNC_ASSERT(condition, message) Debug::CheckAssertion(condition, message)
#else
#define CNC_ASSERT(condition, message)
#endif

namespace cnc {

class Debug {
public:
  static void SetDieOnError(bool die);
  static void SetAbortOnDie(bool abort);
  
  static void Warning(const std::string& message);
  static void Error(const std::string& message);
  static void CheckAssertion(bool assertion, const char* error_message);
  static void CheckAssertion(bool assertion, const std::string& error_message);
  [[noreturn]] static void Die(const std::string& error_message);

private:
  static bool die_on_error_;
  static bool abort_on_die_;
};

}
