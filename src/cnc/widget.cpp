#include "cnc/stdafx.h"
#include "cnc/widget.h"
#include "cnc/game.h"
#include "cnc/mod_data.h"
#include "cnc/widget_loader.h"
#include "cnc/error.h"
#include "cnc/renderer.h"

namespace cnc {

WidgetPtr Ui::LoadWidget(const std::string& id, const WidgetPtr& parent, const WidgetArgs& args) {
  return Game::mod_data()->widget_loader().LoadWidget(args, parent, id);
}

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
    throw Error(MSG("'substitutions' not yet supported"));
  }

  bounds_ = { x, y, width, height };
}

void Widget::PostInit(const WidgetArgs& /*args*/) {
}

void Widget::AddChild(const WidgetPtr& child) {
  child->set_parent(shared_from_this());
  children_.emplace_back(child);
}

const FieldInfo* Widget::GetFieldInfo(const std::string& name) const {
  static const std::map<std::string, FieldInfo> WidgetFieldInfo = {
    { "Id", StringFieldInfo(&Widget::id) },
    { "X", Int32FieldInfo(&Widget::x) },
    { "Y", Int32FieldInfo(&Widget::y) },
    { "Width", Int32FieldInfo(&Widget::width) },
    { "Height", Int32FieldInfo(&Widget::height) }
  };
  auto kvp = WidgetFieldInfo.find(name);
  return (kvp != WidgetFieldInfo.end()) ? &kvp->second : nullptr;
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

}