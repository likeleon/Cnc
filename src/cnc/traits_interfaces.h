#pragma once

#include "cnc/type_exposable.h"
#include "cnc/field_load_info.h"

namespace cnc {

class Actor;

class ITraitInfoInterface {
public:
  virtual ~ITraitInfoInterface() {}
};

class ITraitInfo : public TypeExposable, public ITraitInfoInterface {
public:
  std::vector<std::type_index> Interfaces() const override {
    return{ typeid(ITraitInfo), typeid(ITraitInfoInterface) };
  }

  virtual std::vector<FieldLoadInfo> GetLoadInfo() const = 0;
  virtual std::shared_ptr<void> Create() = 0;

protected:
  template <typename T>
  std::shared_ptr<T> SharedFromBase() {
    return std::static_pointer_cast<T>(shared_from_this());
  }
};

class Requires : public ITraitInfoInterface {
public:
  virtual std::vector<ITraitInfoPtr> RequiredTypes() const = 0;
};

class AttackInfo {
};

class INotifyKilled {
public:
  virtual ~INotifyKilled() {}
  virtual void Killed(Actor& self, const AttackInfo& e) = 0;
};

}
