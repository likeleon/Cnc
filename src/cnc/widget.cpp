#include "cnc/stdafx.h"
#include "cnc/widget.h"
#include "cnc/widget_args.h"
#include "cnc/game.h"
#include "cnc/mod_data.h"
#include "cnc/widget_loader.h"
#include "cnc/error.h"
#include "cnc/renderer.h"
#include "cnc/evaluator.h"
#include "cnc/iinput_handler.h"
#include "cnc/viewport.h"

namespace cnc {

class RootWidget : public ContainerWidget {
public:
  RootWidget() {}
};

Widget::Widget() {
}

Widget::Widget(const Widget& widget)
  : id_(widget.id_),
  x_(widget.x_),
  y_(widget.y_),
  width_(widget.width_),
  height_(widget.height_),
  logic_(widget.logic_),
  visible_(widget.visible_),
  bounds_(widget.bounds_),
  parent_(widget.parent_),
  is_visible_(widget.is_visible_),
  ignore_child_mouse_over_(widget.ignore_child_mouse_over_),
  ignore_mouse_over_(widget.ignore_mouse_over_) {
}

Widget::~Widget() {
}

WidgetPtr Widget::Clone(const WidgetPtr& base) {
  auto widget = base->Clone();
  for (const auto& base_child : base->children_) {
    widget->AddChild(base_child->Clone());
  }
  return widget;
}

WidgetPtr Widget::Clone() const {
  throw Error(MSG("Widget id '" + id_ + "' is not cloneable."));
}

void Widget::Initialize(const WidgetArgs& args) {
  auto parent_bounds = (parent_ == nullptr)
    ? Rectangle(0, 0, Game::renderer()->Resolution().width, Game::renderer()->Resolution().height)
    : parent_->bounds();

  if (args.ContainsKey("substitutions")) {
    throw Error(MSG("WidgetArgs 'substitutions' not yet supported"));
  }

  std::map<std::string, int32_t> substitutions = {
    { "WINDOW_RIGHT", Game::renderer()->Resolution().width },
    { "WINDOW_BOTTOM", Game::renderer()->Resolution().height },
    { "PARENT_RIGHT", parent_bounds.width },
    { "PARENT_LEFT", parent_bounds.Left() },
    { "PARENT_TOP", parent_bounds.Top() },
    { "PARENT_BOTTOM", parent_bounds.height }
  };
  auto width = Evaluator::Evaluate(width_, substitutions);
  auto height = Evaluator::Evaluate(height_, substitutions);

  substitutions.emplace("WIDTH", width);
  substitutions.emplace("HEIGHT", height);

  bounds_ = {
    Evaluator::Evaluate(x_, substitutions),
    Evaluator::Evaluate(y_, substitutions),
    width,
    height
  };
}

void Widget::PostInit(const WidgetArgs& a) {
  if (logic_.empty()) {
    return;
  }

  WidgetArgs args = a;
  args.AddOrAssign("widget", shared_from_this());

  for (const auto& l : logic_) {
    auto logic = Game::mod_data()->object_creator().CreateObject<ChromeLogic>(l, args.args());
    logic_objects_.emplace_back(logic);
  }
}

bool Widget::HasMouseFocus() const {
  return Ui::mouse_focus_widget().get() == this;
}

bool Widget::TakeMouseFocus(const MouseInput& mi) {
  if (HasMouseFocus()) {
    return true;
  }

  if (Ui::mouse_focus_widget() != nullptr && !Ui::mouse_focus_widget()->YieldMouseFocus(mi)) {
    return false;
  }

  Ui::set_mouse_focus_widget(shared_from_this());
  return true;
}

bool Widget::YieldMouseFocus(const MouseInput& /*mi*/) {
  if (Ui::mouse_focus_widget().get() == this) {
    Ui::set_mouse_focus_widget(shared_from_this());
  }
  return true;
}

Rectangle Widget::EventBounds() const {
  return RenderBounds();
}

Rectangle Widget::GetEventBounds() const {
  auto bounds = EventBounds();
  for (const auto& child : children_) {
    if (child->is_visible_()) {
      bounds = Rectangle::Union(bounds, child->GetEventBounds());
    }
  }
  return bounds;
}

void Widget::AddChild(const WidgetPtr& child) {
  child->set_parent(shared_from_this());
  children_.emplace_back(child);
}

void Widget::RemoveChild(WidgetPtr child) {
  if (child != nullptr) {
    children_.erase(std::remove(children_.begin(), children_.end(), child), children_.end());
    child->Removed();
  }
}

void Widget::HideChild(const WidgetPtr& child) {
  if (child != nullptr) {
    children_.erase(std::remove(children_.begin(), children_.end(), child), children_.end());
    child->Hidden();
  }
}

void Widget::RemoveChildren() {
  while (!children_.empty()) {
    RemoveChild(children_.back());
  }
}

void Widget::Hidden() {
  for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
    (*it)->Hidden();
  }
}

void Widget::Removed() {
  for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
    (*it)->Removed();
  }
}

std::string Widget::GetCursor(const Point& /*pos*/) const {
  return "default";
}

std::string Widget::GetCursorOuter(const Point& pos) const {
  if (!(is_visible_() && GetEventBounds().Contains(pos))) {
    return "";
  }

  for (auto riter = children_.cbegin(); riter != children_.end(); ++riter) {
    auto cc = (*riter)->GetCursorOuter(pos);
    if (!cc.empty()) {
      return cc;
    }
  }

  return EventBounds().Contains(pos) ? GetCursor(pos) : "";
}

void Widget::MouseEntered() {
}

void Widget::MouseExited() {
}

bool Widget::HandleMouseInput(const MouseInput& /*mi*/) {
  return false;
}

bool Widget::HandleMouseInputOuter(const MouseInput& mi) {
  if (!(is_visible_() && GetEventBounds().Contains(mi.location))) {
    return false;
  }

  auto old_mouse_over = Ui::mouse_over_widget();

  for (auto riter = children_.crbegin(); riter != children_.crend(); ++riter) {
    if ((*riter)->HandleMouseInputOuter(mi)) {
      return true;
    }
  }

  if (mi.event == MouseInputEvent::Move && Ui::mouse_over_widget() == nullptr) {
    Ui::set_mouse_over_widget(shared_from_this());
  }

  return HandleMouseInput(mi);
}

void Widget::PrepareRenderables() {
}

void Widget::PrepareRenderablesOuter() {
  if (is_visible_()) {
    PrepareRenderables();
    for (const auto& child : children_) {
      child->PrepareRenderablesOuter();
    }
  }
}

void Widget::Draw() {
}

void Widget::DrawOuter() {
  if (is_visible_()) {
    Draw();
    for (const auto& child : children_) {
      child->DrawOuter();
    }
  }
}

WidgetPtr Widget::GetOrNull(const std::string& id) {
  if (id_ == id) {
    return shared_from_this();
  }

  for (const auto& child : children_) {
    auto w = child->GetOrNull(id);
    if (w != nullptr) {
      return w;
    }
  }

  return nullptr;
}

static const std::map<std::string, FieldInfo> WidgetFieldInfo = {
  { "Id", TypeFieldInfo(&Widget::id_) },
  { "X", TypeFieldInfo(&Widget::x_) },
  { "Y", TypeFieldInfo(&Widget::y_) },
  { "Width", TypeFieldInfo(&Widget::width_) },
  { "Height", TypeFieldInfo(&Widget::height_) },
  { "Logic", StringVectorFieldInfo(&Widget::logic_) },
  { "Visible", TypeFieldInfo(&Widget::visible_) },
  { "IgnoreMouseOver", TypeFieldInfo(&Widget::ignore_mouse_over_) },
  { "IgnoreChildMouseOver", TypeFieldInfo(&Widget::ignore_child_mouse_over_) }
};

const std::map<std::string, FieldInfo>& Widget::GetFieldInfoMapCache(const Widget& widget) {
  static std::map<std::type_index, std::map<std::string, FieldInfo>> field_info_map_cache_;
  const auto& type_info = typeid(widget);
  auto iter = field_info_map_cache_.find(type_info);
  if (iter == field_info_map_cache_.end()) {
    std::map<std::string, FieldInfo> map(WidgetFieldInfo);
    for (const auto& f : widget.GetFieldInfoMap()) {
      if (map.find(f.first) != map.end()) {
        throw Error(MSG("Duplicated key found: " + f.first));
      }
      map.emplace(f.first, f.second);
    }
    iter = field_info_map_cache_.emplace(type_info, map).first;
  }
  return iter->second;
}

const FieldInfo* Widget::GetFieldInfo(const std::string& name) const {
  const auto& cache = GetFieldInfoMapCache(*this);
  auto iter = cache.find(name);
  if (iter == cache.end()) {
    return nullptr;
  }
  return &iter->second;
}

std::map<std::string, FieldInfo> Widget::GetFieldInfoMap() const {
  return{};
}

void Widget::set_parent(const WidgetPtr& parent) {
  parent_ = parent;
}

const Widget* Widget::parent() {
  return parent_.get();
}


WidgetPtr Widget::Get(const std::string& id) {
  return Get<Widget>(id);
}

void Widget::set_bounds(const Rectangle& bounds) {
  bounds_ = bounds;
}

const Rectangle& Widget::bounds() const {
  return bounds_;
}

Point Widget::RenderOrigin() const {
  auto offset = (parent_ == nullptr) ? Point::Zero : parent_->ChildOrigin();
  return Point(bounds_.x, bounds_.y) + Size(offset);
}

Point Widget::ChildOrigin() const {
  return RenderOrigin();
}

Rectangle Widget::RenderBounds() const {
  auto ro = RenderOrigin();
  return{ ro.x, ro.y, bounds_.width, bounds_.height };
}

std::map<std::string, FieldInfo> ContainerWidget::GetFieldInfoMap() const {
  return{
    { "ClickThrough", TypeFieldInfo(&ContainerWidget::click_through_) },
  };
}

ContainerWidget::ContainerWidget() {
  ignore_mouse_over_ = true;
}

ContainerWidget::ContainerWidget(const ContainerWidget& other)
: Widget(other) {
  ignore_mouse_over_ = other.ignore_mouse_over_;
}

std::string ContainerWidget::GetCursor(const Point& /*pos*/) const {
  return "";
}

WidgetPtr ContainerWidget::Clone() const {
  return WidgetPtr(new ContainerWidget(*this));
}

bool ContainerWidget::HandleMouseInput(const MouseInput& mi) {
  return !click_through_ && EventBounds().Contains(mi.location);
}

WidgetPtr Ui::root_(new RootWidget());
std::stack<WidgetPtr> Ui::window_list_;
WidgetPtr Ui::mouse_focus_widget_;
WidgetPtr Ui::mouse_over_widget_;

WidgetPtr Ui::LoadWidget(const std::string& id, const WidgetPtr& parent, const WidgetArgs& args) {
  return Game::mod_data()->widget_loader().LoadWidget(args, parent, id);
}

WidgetPtr Ui::OpenWindow(const std::string& id) {
  return OpenWindow(id, {});
}

WidgetPtr Ui::OpenWindow(const std::string& id, const WidgetArgs& args) {
  auto window = Game::mod_data()->widget_loader().LoadWidget(args, root_, id);
  if (!window_list_.empty()) {
    root_->HideChild(window_list_.top());
  }
  window_list_.push(window);
  return window;
}

void Ui::CloseWindow() {
  if (!window_list_.empty()) {
    root_->RemoveChild(window_list_.top());
    window_list_.pop();
  }
  if (!window_list_.empty()) {
    root_->AddChild(window_list_.top());
  }
}

void Ui::ResetAll() {
  root_->RemoveChildren();

  while (!window_list_.empty()) {
    CloseWindow();
  }
}

void Ui::PrepareRenderables() {
  root_->PrepareRenderablesOuter();
}

void Ui::Draw() {
  root_->DrawOuter();
}

bool Ui::HandleInput(const MouseInput& mi) {
  auto was_mouse_over = mouse_over_widget_;

  if (mi.event == MouseInputEvent::Move) {
    mouse_over_widget_ = nullptr;
  }

  bool handled = false;
  if (mouse_focus_widget_ != nullptr && mouse_focus_widget_->HandleMouseInputOuter(mi)) {
    handled = true;
  }

  if (!handled && root_->HandleMouseInputOuter(mi)) {
    handled = true;
  }

  if (mi.event == MouseInputEvent::Move) {
    Viewport::last_mouse_pos_ = mi.location;
    Viewport::ticks_since_last_move_ = 0;
  }

  if (was_mouse_over != mouse_over_widget_) {
    if (was_mouse_over != nullptr) {
      was_mouse_over->MouseExited();
    }

    if (mouse_over_widget_ != nullptr) {
      mouse_over_widget_->MouseEntered();
    }
  }

  return handled;
}

const WidgetPtr& Ui::root() {
  return root_;
}

const WidgetPtr& Ui::mouse_focus_widget() {
  return mouse_focus_widget_;
}

void Ui::set_mouse_focus_widget(const WidgetPtr& w) {
  mouse_focus_widget_ = w;
}

const WidgetPtr& Ui::mouse_over_widget() {
  return mouse_over_widget_;
}

void Ui::set_mouse_over_widget(const WidgetPtr& w) {
  mouse_over_widget_ = w;
}

}