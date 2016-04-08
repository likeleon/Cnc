#include "cnc/stdafx.h"
#include "cnc/traits_interfaces.h"

namespace cnc {

std::deque<std::type_index> ITraitInfo::Types() const {
  return{ typeid(ITraitInfo), typeid(ITraitInfoInterface) };
}

}