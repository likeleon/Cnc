#pragma once

#include "cnc/stream_ptr.h"

namespace cnc {

class CNC_API File {
public:
  static bool Exists(const std::string& path);
  static StreamPtr OpenRead(const std::string& path);  
  static std::string ReadAllText(const std::string& path);
};

}
