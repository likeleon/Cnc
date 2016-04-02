#include "cnc/stdafx.h"
#include "cnc/traits_interfaces.h"

namespace cnc {

std::vector<std::type_index> IDefaultVisibility::Interfaces() const {
  return{ typeid(ITypeExposable) };
}

std::vector<std::type_index> IDefaultVisibility::BaseTypes() const {
  return{};
}

}