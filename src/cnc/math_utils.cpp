#include "cnc/stdafx.h"
#include "cnc/math_utils.h"
#include "cnc/error.h"

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

int32_t ISqrt(int32_t number, ISqrtRoundMode round) {
  if (number < 0) {
    std::ostringstream oss;
    oss << "Attempted to calculate the square root of a negative integer: " << number;
    throw Error(MSG(oss.str()));
  }

  return static_cast<int32_t>(ISqrt(static_cast<uint32_t>(number), round));
}

uint32_t ISqrt(uint32_t number, ISqrtRoundMode round) {
  auto divisor = 1U << 30;

  auto root = 0U;
  auto remainder = number;

  // Find the highest term in the divisor
  while (divisor > number) {
    divisor >>= 2;
  }

  while (divisor != 0) {
    if (root + divisor <= remainder) {
      remainder -= root + divisor;
      root += 2 * divisor;
    }

    root >>= 1;
    divisor >>= 2;
  }
  
  if (round == ISqrtRoundMode::Nearest && remainder > root) {
    root += 1;
  } else if (round == ISqrtRoundMode::Ceiling && root * root < number) {
    root += 1;
  }

  return root;
}

}