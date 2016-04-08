#pragma once

#include "cnc/itype_exposable.h"
#include "cnc/field_load_info.h"

namespace cnc {

class Actor;
class Player;
class ActorInitializer;
class WorldRenderer;

class ITraitInfoInterface {
public:
  virtual ~ITraitInfoInterface() {}
};

class CNC_API ITraitInfo : public ITypeExposable, public ITraitInfoInterface {
public:
  std::deque<std::type_index> Types() const override;

  virtual std::vector<FieldLoadInfo> GetLoadInfo() const = 0;
  virtual TypeExposablePtr Create(const ActorInitializer& init) = 0;

protected:
  template <typename T>
  std::shared_ptr<T> SharedFromBase() {
    return std::static_pointer_cast<T>(shared_from_this());
  }
};

template <typename T>
class TraitInfo : public ITraitInfo {
public:
  std::deque<std::type_index> Types() const override {
    auto types = ITraitInfo::Types();
    types.push_front(typeid(TraitInfo<T>));
    return types;
  }

  TypeExposablePtr Create(const ActorInitializer& /*init*/) override {
    return std::make_shared<T>();
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

class IDefaultVisibility {
public:
  virtual bool IsVisible(Actor& self, Player& by_player) = 0;
};

class ILoadsPalettes {
public:
  virtual void LoadPalettes(WorldRenderer& wr) = 0;
};

}
