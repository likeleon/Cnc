#pragma once

namespace cnc {

enum class ISqrtRoundMode {
  Floor,
  Nearest,
  Ceiling
};

int32_t NextPowerOf2(int32_t v);
bool IsPowerOf2(int32_t v);
int32_t ISqrt(int32_t number, ISqrtRoundMode round = ISqrtRoundMode::Floor);
uint32_t ISqrt(uint32_t number, ISqrtRoundMode round = ISqrtRoundMode::Floor);

}
