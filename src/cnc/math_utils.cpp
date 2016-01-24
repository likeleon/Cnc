#include "cnc/stdafx.h"
#include "math_utils.h"

namespace cnc {

int32_t NextPowerOf2(int32_t v) {
  --v;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  ++v;
  return v;
}

bool IsPowerOf2(int32_t v) {
  return (v & (v - 1)) == 0;
}

}