#pragma once

#pragma once

namespace cnc {
namespace mods {
namespace common {

class FastByteReader {
public:
  FastByteReader(const std::vector<char>& src, int32_t offset = 0);

  bool Done();
  char ReadByte();
  int32_t ReadWord();
  void CopyTo(std::vector<char>& dest, int32_t offset, int32_t count);
  int32_t Remaining() const;

private:
  const std::vector<char>& src_;
  int32_t offset_;
};

}
}
}

