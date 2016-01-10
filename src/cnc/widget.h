#pragma once

#include "cnc/any.h"
#include "cnc/widget_ptr.h"
#include "cnc/rectangle.h"
#include "cnc/field_loader.h"
#include "cnc/chrome_logic.h"

namespace cnc {

class WidgetArgs;

class CNC_API Widget : public std::enable_shared_from_this<Widget> {
public:
  Widget();

  Widget(const Widget&) = delete;
  Widget& operator=(const Widget&) = delete;

  virtual void Initialize(const WidgetArgs& args);
  virtual void PostInit(const WidgetArgs& args);
  virtual void AddChild(const WidgetPtr& child);
  virtual void RemoveChild(const WidgetPtr& child);
  virtual void HideChild(const WidgetPtr& child);
  virtual void RemoveChildren();
  virtual void Hidden();
  virtual void Removed();

  virtual void PrepareRenderables();
  virtual void PrepareRenderablesOuter();
  virtual void Draw();
  virtual void DrawOuter();

  const Rectangle& bounds() const;
  virtual Point RenderOrigin() const;
  virtual Point ChildOrigin() const;
  virtual Rectangle RenderBounds() const;

  void set_parent(const Widget* parent);
  const Widget* parent();

  const FieldInfo* GetFieldInfo(const std::string& name) const;

  std::string id_;
  std::string x_ = "0";
  std::string y_ = "0";
  std::string width_ = "0";
  std::string height_ = "0";
  std::vector<std::string> logic_;
  std::vector<ChromeLogicUniquePtr> logic_objects_;
  bool visible_ = true;
  bool ignore_mouse_over_ = false;
  bool ignore_child_mouse_over_ = false;

  std::function<bool()> IsVisible = [this] { return visible_; };

protected:
  virtual std::map<std::string, FieldInfo> GetFieldInfoMap() const;

private:
  static const std::map<std::string, FieldInfo>& GetFieldInfoMapCache(const Widget& widget);
    
  const Widget* parent_;
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

  static void PrepareRenderables();
  static void Draw();

  static const WidgetPtr& root();

private:
  static WidgetPtr root_;
  static std::stack<WidgetPtr> window_list_;
};

}
