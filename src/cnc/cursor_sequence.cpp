#include "cnc/stdafx.h"
#include "cnc/cursor_sequence.h"
#include "cnc/mini_yaml.h"

namespace cnc {

CursorSequence::CursorSequence(FrameCache& cache, const std::string& name, const std::string& cursor_src, 
                               const std::string& palette, const MiniYaml& info) {
  auto d = info.ToMap();

  start_ = std::stoi(d.at("Start").value());
  palette_ = palette;
  name_ = name;

  if (d.find("Length") != d.end()) {
    length_ = std::stoi(d.at("Length").value());
  } else if (d.find("End") != d.end()) {
    length_ = std::stoi(d.at("End").value());
  } else {
    length_ = 1;
  }

  frames_.assign(cache[cursor_src].begin() + start_,
                 cache[cursor_src].begin() + start_ + length_);

  if (d.find("X") != d.end()) {
    hotspot_.x = std::stoi(d.at("X").value());
  }

  if (d.find("Y") != d.end()) {
    hotspot_.y = std::stoi(d.at("Y").value());
  }
}

std::string CursorSequence::name() const {
  return name_;
}

int32_t CursorSequence::start() const {
  return start_;
}

int32_t CursorSequence::length() const {
  return length_;
}

const std::string& CursorSequence::palette() const {
  return palette_;
}

const Point& CursorSequence::hotspot() const {
  return hotspot_;
}

const std::vector<ISpriteFramePtr>& CursorSequence::frames() const {
  return frames_;
}

}
