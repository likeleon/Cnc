#include "cnc/stdafx.h"
#include "cnc/trait_dictionary.h"
#include "cnc/container_utils.h"

namespace cnc {

void TraitDictionary::AddTrait(ActorPtr actor, TypeExposablePtr val) {
  for (const auto& i : val->Interfaces()) {
    InnerAdd(actor, i, val);
  }
  for (const auto& tt : val->BaseTypes()) {
    InnerAdd(actor, tt, val);
  }
}

void TraitDictionary::CheckDestroyed(const Actor& actor) {
}

}