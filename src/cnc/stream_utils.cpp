#include "cnc/stdafx.h"
#include "cnc/stream_utils.h"

namespace cnc {

void StreamUtils::Copy(std::istream& from, std::ostream& to, size_t buffer_size) {
  std::vector<char> buff(buffer_size);
  do {
    from.read(buff.data(), buff.size());
    to.write(buff.data(), from.gcount());
  } while (static_cast<size_t>(from.gcount()) == buff.size());
}

}