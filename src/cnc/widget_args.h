#pragma once

namespace cnc {

class Any;

class CNC_API WidgetArgs {
public:
  WidgetArgs();
  WidgetArgs(const std::map<std::string, Any>& args);

  void Add(const std::string& key, Any&& value);
  void AddOrAssign(const std::string& key, Any&& value);
  bool Remove(const std::string& key);
  bool ContainsKey(const std::string& key) const;

  const std::map<std::string, Any>& args() const;

private:
  std::map<std::string, Any> args_;
};

}
