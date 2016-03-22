#include "cnc/stdafx.h"
#include "cnc/order_manager.h"
#include "cnc/world.h"

namespace cnc {

OrderManager::OrderManager(const std::string& host, int32_t port, const std::string& password)
  : host_(host), port_(port), password_(password) {
}

OrderManager::~OrderManager() {
}

void OrderManager::set_world(std::unique_ptr<World> world) {
  world_ = std::move(world);
}

}