#pragma once

#include "cnc/sprite_loader.h"
#include "cnc/point.h"

namespace cnc {

class FrameCache;
class MiniYaml;

class CursorSequence {
public:
  CursorSequence(FrameCache& cache, const std::string& name, const std::string& cursor_src,
                 const std::string& palette, const MiniYaml& info);

  CursorSequence(const CursorSequence&) = delete;
  CursorSequence& operator=(const CursorSequence&) = delete;
  
  std::string name() const;
  int32_t start() const;
  int32_t length() const;
  const Point& hotspot() const;
  const std::vector<ISpriteFramePtr>& frames() const;

private:
  std::string name_;
  int32_t start_;
  int32_t length_;
  std::string palette_;
  Point hotspot_ = Point::Zero;
  std::vector<ISpriteFramePtr> frames_;
};

}
