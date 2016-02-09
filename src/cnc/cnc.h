#pragma once

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

namespace cnc {

#ifdef CNC_EXPORTS
#define CNC_API __declspec(dllexport)
#else
#define CNC_API __declspec(dllimport)
#endif

#  pragma warning(disable : 4251 4275)
// 4251 needs to have dll-interface to be used by clients of class
// 4275 non dll-interface class 'Foo::Bar' used as base for dll-interface class 'Foo::Bar'

}
