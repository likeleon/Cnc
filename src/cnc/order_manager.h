#pragma once

namespace cnc {

class World;

class OrderManager {
public:
  OrderManager(const std::string& host, int32_t port, const std::string& password);
  ~OrderManager();

  void set_world(std::unique_ptr<World> world);

private:
  std::unique_ptr<World> world_;
  std::string host_;
  int32_t port_;
  std::string password_;
};

}
