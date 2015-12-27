#include "cnc/stdafx.h"
#include "cnc/mapped_image.h"
#include "cnc/sprite.h"
#include "cnc/field_loader.h"

namespace cnc {

MappedImage::MappedImage(const std::string& src, const MiniYaml& info)
  : src_(src) {
  FieldLoader::LoadField(*this, TypeFieldInfo(&MappedImage::rect_), info.value());
}

Sprite MappedImage::GetImage(const SheetPtr& s) const {
  return{ s, rect_, TextureChannel::Alpha };
}

const std::string& MappedImage::src() const {
  return src_;
}

const Rectangle& MappedImage::rect() const {
  return rect_;
}

}
