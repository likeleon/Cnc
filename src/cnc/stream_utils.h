#pragma once

namespace cnc {

class CNC_API StreamUtils {
public:
  static void Copy(std::istream& from, std::ostream& to, size_t buffer_size = 1024 * 1024);
};

}
