#include "cnc/stdafx.h"
#include "cnc/widget_args.h"
#include "cnc/any.h"
#include "cnc/error.h"

namespace cnc {

WidgetArgs::WidgetArgs() {
}

WidgetArgs::WidgetArgs(const std::map<std::string, Any>& args)
  : args_(args) {
}

void WidgetArgs::Add(const std::string& key, Any&& value) {
  if (ContainsKey(key)) {
    throw Error(MSG("key '" + key + "' already added"));
  }
  args_.emplace(key, std::forward<Any>(value));
}

void WidgetArgs::AddOrAssign(const std::string& key, Any&& value) {
  args_.insert_or_assign(key, std::forward<Any>(value));
}

bool WidgetArgs::Remove(const std::string& key) {
  return (args_.erase(key) != 0);
}

bool WidgetArgs::ContainsKey(const std::string& key) const {
  return args_.find(key) != args_.end();
}

const std::map<std::string, Any>& WidgetArgs::args() const {
  return args_;
}

}