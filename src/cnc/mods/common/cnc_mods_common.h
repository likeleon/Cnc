#pragma once

#include "cnc/cnc.h"

namespace cnc {
namespace mods {
namespace common {

#ifdef CNC_MODS_COMMON_EXPORTS
#define CNC_MODS_COMMON_API __declspec(dllexport)
#else
#define CNC_MODS_COMMON_API __declspec(dllimport)
#endif

}
}
}