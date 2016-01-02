#pragma once

namespace cnc {

struct CNC_API Message {
  std::string text;
  const char* file_name;
  uint32_t line_no;
  const char* function_name;
};

std::ostream& operator<<(std::ostream& os, const Message& msg);

class CNC_API MessageBuilder {
public:
  MessageBuilder(const std::string& text,
                 const char* file_name,
                 uint32_t line_no, 
                 const char* function_name);

  inline operator const Message& () const { return message; }
  inline operator Message& () { return message; }

private:
  Message message;
};

#define MSG(text) ::cnc::MessageBuilder(text, __FILE__, __LINE__, __FUNCSIG__)

}
