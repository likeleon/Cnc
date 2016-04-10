#pragma once

#include "cnc/rectangle.h"

namespace cnc {

class MiniYaml;
struct Sprite;

class MappedImage {
public:
  MappedImage(const std::string& default_src, const MiniYaml& info);

  Sprite GetImage(const SheetPtr& s) const;

  const std::string& src() const;
  const Rectangle& rect() const;

private:
  std::string src_;
  Rectangle rect_;
};

}
