#pragma once

#include "cnc/any.h"

namespace cnc {

class WidgetArgs {
public:
  WidgetArgs();
  WidgetArgs(const std::map<std::string, Any>& args);

  void Add(const std::string& key, const Any& value);

private:
  std::map<std::string, Any> args_;
};

}
