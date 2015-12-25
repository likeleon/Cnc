#include "cnc/stdafx.h"
#include "math_utils.h"

namespace cnc {

bool IsPowerOf2(int32_t v) {
  return (v & (v - 1)) == 0;
}

}