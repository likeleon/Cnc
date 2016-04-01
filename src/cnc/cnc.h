#pragma once

#define NOMINMAX
#include <windows.h>
#include <map>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <fstream>
#include <chrono>
#include <memory>
#include <typeindex>
#include <array>
#include <stack>
#include <functional>

#include <SDL.h>
#include <SDL_ttf.h>

#pragma warning(disable:4702)
#include "cnc/optional.h"
#pragma warning(default:4702)

namespace cnc {

#ifdef CNC_EXPORTS
#define CNC_API __declspec(dllexport)
#else
#define CNC_API __declspec(dllimport)
#endif

template <typename T>
using optional = std::experimental::optional<T>;

#  pragma warning(disable : 4251 4275)
// 4251 needs to have dll-interface to be used by clients of class
// 4275 non dll-interface class 'Foo::Bar' used as base for dll-interface class 'Foo::Bar'

class ITraitInfo;
using ITraitInfoPtr = std::shared_ptr<ITraitInfo>;

}
