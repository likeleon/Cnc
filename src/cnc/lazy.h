#pragma once

#include "cnc/optional.h"

namespace cnc {

template<typename F>
class Lazy {
public:
  typedef typename std::result_of<F()>::type ResultType;

  explicit Lazy(F f)
    : func_(f) {
  }

  const ResultType& operator()() const {
    return const_cast<Lazy&>(*this)();
  }

  ResultType& operator()() {
    if (!value_) {
      value_ = func_();
    }
    return *value_;
  }

  Lazy(const Lazy&) = delete;
  Lazy& operator=(const Lazy&) = delete;
  Lazy& operator=(Lazy&&) = delete;

private:
  optional<ResultType> value_;
  F func_;
};

}
