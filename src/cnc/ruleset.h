#pragma once

#include "cnc/actor_info_ptr.h"

namespace cnc {

class Ruleset {
public:
  Ruleset(std::map<std::string, ActorInfoPtr>&& actors);
  
private:
  std::map<std::string, ActorInfoPtr> actors_;
};

}
