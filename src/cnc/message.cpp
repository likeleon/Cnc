#include "cnc/stdafx.h"
#include "cnc/message.h"
#include "cnc/stop_watch.h"

namespace cnc {

std::ostream& operator<<(std::ostream& os, const Message& msg) {
  os << msg.file_name << ":" << msg.line_no << " ";
  os << "(" << msg.function_name << ")";
  os << ": " << msg.text;
  return os;
}

MessageBuilder::MessageBuilder(const std::string& text,
                               const char* file_name, 
                               uint32_t line_no, 
                               const char* function_name) {
  message.text = text;
  message.file_name = file_name;
  message.line_no = line_no;
  message.function_name = function_name;
}

}