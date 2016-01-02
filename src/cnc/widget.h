#pragma once

#include "cnc/any.h"
#include "cnc/widget_ptr.h"
#include "cnc/rectangle.h"
#include "cnc/field_loader.h"

namespace cnc {

class CNC_API WidgetArgs {
public:
  WidgetArgs();
  WidgetArgs(const std::map<std::string, Any>& args);

  void Add(const std::string& key, Any&& value);
  bool Remove(const std::string& key);
  bool ContainsKey(const std::string& key) const;

  const std::map<std::string, Any>& args() const;

private:
  std::map<std::string, Any> args_;
};

class CNC_API Widget : public std::enable_shared_from_this<Widget> {
public:  
  virtual void Initialize(const WidgetArgs& args);
  virtual void PostInit(const WidgetArgs& args);
  virtual void AddChild(const WidgetPtr& child);
  virtual void RemoveChild(const WidgetPtr& child);
  virtual void HideChild(const WidgetPtr& child);
  virtual void RemoveChildren();
  virtual void Hidden();
  virtual void Removed();

  void set_parent(WidgetPtr parent);
  Widget* parent();

  const Rectangle& bounds() const;
  const FieldInfo* GetFieldInfo(const std::string& name) const;

  std::string id_;
  std::string x_ = "0";
  std::string y_ = "0";
  std::string width_ = "0";
  std::string height_ = "0";
  std::vector<std::string> logic_;
  bool ignore_mouse_over_ = false;
  bool ignore_child_mouse_over_ = false;

protected:
  virtual std::map<std::string, FieldInfo> GetFieldInfoMap() const;

private:
  static const std::map<std::string, FieldInfo>& GetFieldInfoMapCache(const Widget& widget);
    
  WidgetPtr parent_;
  Rectangle bounds_;
  std::vector<WidgetPtr> children_;
};

class CNC_API ContainerWidget : public Widget {
};

class CNC_API Ui {
public:
  Ui() = delete;

  static WidgetPtr LoadWidget(const std::string& id, const WidgetPtr& parent, const WidgetArgs& args);
  static WidgetPtr OpenWindow(const std::string& id);
  static WidgetPtr OpenWindow(const std::string& id, const WidgetArgs& args);
  static void CloseWindow();

  static void ResetAll();

  static const WidgetPtr& root();

private:
  static WidgetPtr root_;
  static std::stack<WidgetPtr> window_list_;
};

}
