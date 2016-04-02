#pragma once

namespace cnc {

class ITypeExposable : public std::enable_shared_from_this<ITypeExposable> {
public:
  virtual ~ITypeExposable() {}
  virtual std::vector<std::type_index> Types() const = 0;
};

}
