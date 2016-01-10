#pragma once

#include "cnc/error.h"

namespace cnc {

class Any {
public:
  Any() {
  }

  template <typename ValueType>
  Any(const ValueType& value)
    : content_(new Holder<ValueType>(value)) {
  }

  Any(const Any& other)
    : content_(other.content_ != nullptr ? other.content_->Clone() : nullptr) {
  }

  template <typename ValueType>
  Any(ValueType&& value)
    : content_(new Holder<ValueType>(std::move(value))) {
  }

  Any(Any&& other)
    : content_(other.content_) {
    other.content_ = nullptr;
  }

  ~Any() {
    delete content_;
  }

  Any& Swap(Any& rhs) {
    std::swap(content_, rhs.content_);
    return *this;
  }

  Any& operator=(const Any& rhs) {
    Any(rhs).Swap(*this);
    return *this;
  }

  Any& operator=(Any&& rhs) {
    rhs.Swap(*this);
    Any().Swap(rhs);
    return *this;
  }

  template <typename ValueType>
  Any& operator=(ValueType&& rhs) {
    Any(std::move(rhs)).Swap(*this);
    return *this;
  }

  bool Empty() const {
    return content_ != nullptr;
  }

  void Clear() {
    Any().Swap(*this);
  }

  const std::type_info& type() const {
    if (content_ == nullptr) {
      return typeid(void);
    }
    return content_->type();
  }

private:
  class PlaceHolder {
  public:
    virtual ~PlaceHolder() {}
    virtual const type_info& type() const = 0;
    virtual PlaceHolder* Clone() const = 0;
  };

  template <typename ValueType>
  class Holder : public PlaceHolder {
  public:
    Holder(const ValueType& value)
      : value_(value) {
    }

    Holder(ValueType&& value)
      : value_(std::move(value)) {
    }

    const type_info& type() const override {
      return typeid(ValueType);
    }

    PlaceHolder* Clone() const override {
      return new Holder(value_);
    }

    ValueType value_;
  };

  PlaceHolder* content_ = nullptr;

  template <typename ValueType>
  friend ValueType* AnyCast(Any*);
};

inline void Swap(Any& lhs, Any& rhs) {
  lhs.Swap(rhs);
}

template <typename ValueType>
ValueType* AnyCast(Any* any) {
  if (any == nullptr || any->type() != typeid(ValueType)) {
    return nullptr;
  }
  return &static_cast<Any::Holder<ValueType>*>(any->content_)->value_;
}

template <typename ValueType>
ValueType AnyCast(Any& any) {
  using NonRef = std::remove_reference_t<ValueType>;
  NonRef* result = AnyCast<NonRef>(&any);
  if (result == nullptr) {
    throw Error(MSG("Failed conversion using AnyCast"));
  }
  return *result;
}

template <typename ValueType>
ValueType AnyCast(const Any& any) {
  using NonRef = std::remove_reference_t<ValueType>;
  return AnyCast<const NonRef&>(const_cast<Any&>(any));
}

}
