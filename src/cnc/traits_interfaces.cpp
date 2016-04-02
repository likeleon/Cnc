#include "cnc/stdafx.h"
#include "cnc/traits_interfaces.h"

namespace cnc {

std::vector<std::type_index> ITraitInfo::Types() const {
  return{ typeid(ITraitInfo), typeid(ITraitInfoInterface) };
}

std::vector<std::type_index> IDefaultVisibility::Types() const {
  return{ typeid(IDefaultVisibility) };
}

}