#include "cnc/stdafx.h"
#include "cnc/widget.h"

namespace cnc {

WidgetArgs::WidgetArgs() {
}

WidgetArgs::WidgetArgs(const std::map<std::string, Any>& args)
  : args_(args) {
}

void WidgetArgs::Add(const std::string& key, const Any& value) {
  args_.emplace(key, value);
}


}