#pragma once

#include "cnc/itype_exposable.h"
#include "cnc/field_load_info.h"

namespace cnc {

class Actor;
class Player;

class ITraitInfoInterface {
public:
  virtual ~ITraitInfoInterface() {}
};

class CNC_API ITraitInfo : public ITypeExposable, public ITraitInfoInterface {
public:
  std::vector<std::type_index> Types() const override;

  virtual std::vector<FieldLoadInfo> GetLoadInfo() const = 0;
  virtual TypeExposablePtr Create() = 0;

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
  virtual void Killed(Actor& self, const AttackInfo& e) = 0;
};

class IDefaultVisibilityInfo : public ITraitInfoInterface {};

class IDefaultVisibility : public ITypeExposable {
public:
  virtual bool IsVisible(Actor& self, Player& by_player) = 0;

  std::vector<std::type_index> Types() const override;
};

}
