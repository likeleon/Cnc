#pragma once

#include "cnc/type_exposable.h"
#include "cnc/field_load_info.h"

namespace cnc {

class ITraitInfoInterface {
public:
  virtual ~ITraitInfoInterface() {}
};

class ITraitInfo : public TypeExposable, public ITraitInfoInterface {
public:
  virtual void* Create() = 0;
  virtual std::vector<FieldLoadInfo> GetLoadInfo() const = 0;
};

using ITraitInfoPtr = std::shared_ptr<ITraitInfo>;

}
