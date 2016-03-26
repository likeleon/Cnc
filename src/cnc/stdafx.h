// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <SDKDDKVer.h>

#include <stdio.h>
#include <tchar.h>

#include <cassert>
#include <cctype>
#include <exception>
#include <filesystem>
#include <functional>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <typeindex>
#include <thread>
#include <chrono>
#include <array>
#include <ctime>
#include <sstream>
#include <stack>
#include <type_traits>

#define GLEW_STATIC
#include "GL/glew.h"

#include "SDL.h"
#undef main
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "cnc/cnc.h"


