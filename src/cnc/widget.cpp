#include "cnc/stdafx.h"
#include "cnc/widget.h"
#include "cnc/game.h"
#include "cnc/mod_data.h"
#include "cnc/widget_loader.h"
#include "cnc/error.h"
#include "cnc/renderer.h"
#include "cnc/evaluator.h"

namespace cnc {

class RootWidget : public ContainerWidget {
};

WidgetPtr Ui::root_ = std::make_shared<RootWidget>();
std::stack<WidgetPtr> Ui::window_list_;

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

bool WidgetArgs::Remove(const std::string& key) {
  return (args_.erase(key) != 0);
}

bool WidgetArgs::ContainsKey(const std::string& key) const {
  return args_.find(key) != args_.end();
}

const std::map<std::string, Any>& WidgetArgs::args() const {
  return args_;
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

void Widget::PostInit(const WidgetArgs& /*args*/) {
}

void Widget::AddChild(const WidgetPtr& child) {
  child->set_parent(shared_from_this());
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

const FieldInfo* Widget::GetFieldInfo(const std::string& name) const {
  static const std::map<std::string, FieldInfo> WidgetFieldInfo = {
    { "Id", StringFieldInfo(&Widget::id_) },
    { "X", StringFieldInfo(&Widget::x_) },
    { "Y", StringFieldInfo(&Widget::y_) },
    { "Width", StringFieldInfo(&Widget::width_) },
    { "Height", StringFieldInfo(&Widget::height_) }
  };
  auto kvp = WidgetFieldInfo.find(name);
  if (kvp != WidgetFieldInfo.end()) {
    return &kvp->second;
  }
  return OnGetFieldInfo(name);
}

const FieldInfo* Widget::OnGetFieldInfo(const std::string& /*name*/) const {
  return nullptr;
}

void Widget::set_parent(WidgetPtr parent) {
  parent_ = parent;
}

Widget* Widget::parent() {
  return parent_.get();
}

const Rectangle& Widget::bounds() const {
  return bounds_;
}

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

const WidgetPtr& Ui::root() {
  return root_;
}

}