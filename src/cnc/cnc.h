#pragma once

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
