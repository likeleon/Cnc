#include "cnc/mods/common/stdafx.h"
#include "cnc/mods/common/image_widget.h"

namespace cnc {
namespace mods {
namespace common {

std::map<std::string, FieldInfo> ImageWidget::GetFieldInfoMap() const {
  return{
    { "ImageCollection", TypeFieldInfo(&ImageWidget::image_collection_) },
    { "ImageName", TypeFieldInfo(&ImageWidget::image_name_) }
  };
}

}
}
}
