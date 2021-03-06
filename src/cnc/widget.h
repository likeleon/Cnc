#pragma once

#include "cnc/any.h"
#include "cnc/rectangle.h"
#include "cnc/field_load_info.h"
#include "cnc/chrome_logic.h"
#include "cnc/action.h"

namespace cnc {

class WidgetArgs;
struct MouseInput;


class CNC_API Widget : public std::enable_shared_from_this<Widget> {
public:
  Widget();
  virtual ~Widget();

  Widget(const Widget& widget);
  Widget& operator=(const Widget&) = delete;

  static WidgetPtr Clone(const WidgetPtr& base);
  virtual WidgetPtr Clone() const;

  virtual void Initialize(const WidgetArgs& args);
  virtual void PostInit(const WidgetArgs& args);

  bool HasMouseFocus() const;
  virtual bool TakeMouseFocus(const MouseInput& mi);
  virtual bool YieldMouseFocus(const MouseInput& mi);

  virtual Rectangle EventBounds() const;
  virtual Rectangle GetEventBounds() const;

  virtual void AddChild(const WidgetPtr& child);
  virtual void RemoveChild(WidgetPtr child);
  virtual void HideChild(const WidgetPtr& child);
  virtual void RemoveChildren();
  
  virtual void Hidden();
  virtual void Removed();

  virtual std::string GetCursor(const Point& pos) const;
  std::string GetCursorOuter(const Point& pos) const;

  virtual void MouseEntered();
  virtual void MouseExited();
  virtual bool HandleMouseInput(const MouseInput& mi);
  bool HandleMouseInputOuter(const MouseInput& mi);

  virtual void PrepareRenderables();
  virtual void PrepareRenderablesOuter();
  virtual void Draw();
  virtual void DrawOuter();

  WidgetPtr GetOrNull(const std::string& id);

  template <typename T>
  std::shared_ptr<T> GetOrNull(const std::string& id);

  template <typename T>
  std::shared_ptr<T> Get(const std::string& id);

  WidgetPtr Get(const std::string& id);

  void set_bounds(const Rectangle& bounds);
  const Rectangle& bounds() const;
  virtual Point RenderOrigin() const;
  virtual Point ChildOrigin() const;
  virtual Rectangle RenderBounds() const;

  void set_parent(const WidgetPtr& parent);
  const Widget* parent();

  const FieldInfo* GetFieldInfo(const std::string& name) const;

  std::string id_;
  std::string x_ = "0";
  std::string y_ = "0";
  std::string width_ = "0";
  std::string height_ = "0";
  std::vector<std::string> logic_;
  std::vector<ChromeLogicPtr> logic_objects_;
  bool visible_ = true;
  bool ignore_mouse_over_ = false;
  bool ignore_child_mouse_over_ = false;

  std::function<bool()> is_visible_ = [this] { return visible_; };

protected:
  virtual std::map<std::string, FieldInfo> GetFieldInfoMap() const;

private:
  static const std::map<std::string, FieldInfo>& GetFieldInfoMapCache(const Widget& widget);
    
  WidgetPtr parent_;
  Rectangle bounds_;
  std::vector<WidgetPtr> children_;
};

class CNC_API ContainerWidget : public Widget {
public:
  ContainerWidget();
  ContainerWidget(const ContainerWidget& other);

  std::string GetCursor(const Point& pos) const override;
  WidgetPtr Clone() const override;
  bool HandleMouseInput(const MouseInput& mi) override;
  
  std::map<std::string, FieldInfo> GetFieldInfoMap() const override;

  bool click_through_ = true;
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

  static bool HandleInput(const MouseInput& mi);

  static const WidgetPtr& root();

  static const WidgetPtr& mouse_focus_widget();
  static void set_mouse_focus_widget(const WidgetPtr& w);
  static const WidgetPtr& mouse_over_widget();
  static void set_mouse_over_widget(const WidgetPtr& w);

private:
  static WidgetPtr root_;
  static std::stack<WidgetPtr> window_list_;
  static WidgetPtr mouse_focus_widget_;
  static WidgetPtr mouse_over_widget_;
};

template <typename T>
std::shared_ptr<T> Widget::GetOrNull(const std::string& id) {
  return std::static_pointer_cast<T>(GetOrNull(id));
}

template <typename T>
std::shared_ptr<T> Widget::Get(const std::string& id) {
  auto t = GetOrNull<T>(id);
  if (t == nullptr) {
    std::ostringstream oss;
    oss << "Widget " << id_ << " has no child " << id << " of type " << typeid(T).name();
    throw Error(MSG(oss.str()));
  }
  return t;
}

}
