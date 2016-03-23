#include "cnc/stdafx.h"
#include "cnc/ruleset.h"

namespace cnc {

Ruleset::Ruleset(std::map<std::string, ActorInfoPtr>&& actors)
  : actors_(std::move(actors)) {
}

}