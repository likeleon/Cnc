#pragma once

namespace cnc {

class Evaluator {
public:
  Evaluator() = delete;

  static int32_t Evaluate(const std::string& expr, const std::map<std::string, int32_t>& syms);
};

}
