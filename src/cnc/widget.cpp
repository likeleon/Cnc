#include "cnc/stdafx.h"
#include "cnc/widget.h"
#include "cnc/widget_args.h"
#include "cnc/game.h"
#include "cnc/mod_data.h"
#include "cnc/widget_loader.h"
#include "cnc/error.h"
#include "cnc/renderer.h"
#include "cnc/evaluator.h"

namespace cnc {

class RootWidget : public ContainerWidget {
public:
  RootWidget() {}
};

Widget::Widget() {
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
    auto logic_object = Game::mod_data()->object_creator().CreateObject<ChromeLogic>(l, args.args());
    logic_objects_.emplace_back(std::move(logic_object));
  }
}

void Widget::AddChild(const WidgetPtr& child) {
  child->set_parent(this);
  children_.emplace_back(child);
}

void Widget::RemoveChild(const WidgetPtr& child) {
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

void Widget::PrepareRenderables() {
}

void Widget::PrepareRenderablesOuter() {
  if (IsVisible()) {
    PrepareRenderables();
    for (const auto& child : children_) {
      child->PrepareRenderablesOuter();
    }
  }
}

void Widget::Draw() {
}

void Widget::DrawOuter() {
  if (IsVisible()) {
    Draw();
    for (const auto& child : children_) {
      child->DrawOuter();
    }
  }
}

WidgetPtr Widget::GetOrNull(const std::string id) {
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
  { "Id", StringFieldInfo(&Widget::id_) },
  { "X", StringFieldInfo(&Widget::x_) },
  { "Y", StringFieldInfo(&Widget::y_) },
  { "Width", StringFieldInfo(&Widget::width_) },
  { "Height", StringFieldInfo(&Widget::height_) },
  { "Logic", StringVectorFieldInfo(&Widget::logic_) },
  { "IgnoreMouseOver", BoolFieldInfo(&Widget::ignore_mouse_over_) },
  { "IgnoreChildMouseOver", BoolFieldInfo(&Widget::ignore_child_mouse_over_) }
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

void Widget::set_parent(const Widget* parent) {
  parent_ = parent;
}

const Widget* Widget::parent() {
  return parent_;
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

WidgetPtr Ui::root_(new RootWidget());
std::stack<WidgetPtr> Ui::window_list_;

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

const WidgetPtr& Ui::root() {
  return root_;
}

}