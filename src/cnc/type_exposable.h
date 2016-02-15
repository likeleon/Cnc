#pragma once

namespace cnc {

class TypeExposable : public std::enable_shared_from_this<TypeExposable> {
public:
  virtual ~TypeExposable() {}

  virtual std::vector<std::type_index> Interfaces() const = 0;
  virtual std::vector<std::type_index> BaseTypes() const = 0;
};

using TypeExposablePtr = std::shared_ptr<TypeExposable>;

}
