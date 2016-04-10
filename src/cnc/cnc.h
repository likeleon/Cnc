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

class ITypeExposable;
using TypeExposablePtr = std::shared_ptr<ITypeExposable>;

class TileSet;
using TileSetPtr = std::shared_ptr<TileSet>;

class ITrait;
using ITraitPtr = std::shared_ptr<ITrait>;

class Actor;
using ActorPtr = std::shared_ptr<Actor>;

class ActorInfo;
using ActorInfoPtr = std::shared_ptr<ActorInfo>;

class Map;
using MapUniquePtr = std::unique_ptr<Map>;

class ImmutablePalette;
using ImmutablePalettePtr = std::shared_ptr<ImmutablePalette>;

class MutablePalette;
using MutablePalettePtr = std::shared_ptr<MutablePalette>;

class Sheet;
using SheetPtr = std::shared_ptr<Sheet>;

class ISpriteFrame;
using ISpriteFramePtr = std::shared_ptr<ISpriteFrame>;

class ISpriteLoader;
using SpriteLoaderPtr = std::shared_ptr<ISpriteLoader>;

class Stream;
using StreamPtr = std::shared_ptr<Stream>;

struct SDL_Surface_Deleter {
  void operator()(SDL_Surface* sdl_surface) {
    SDL_FreeSurface(sdl_surface);
  }
};
using SDL_Surface_UniquePtr = std::unique_ptr<SDL_Surface, SDL_Surface_Deleter>;

class Widget;
using WidgetPtr = std::shared_ptr<Widget>;

}
