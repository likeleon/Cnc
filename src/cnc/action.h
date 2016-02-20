#pragma once

namespace cnc {

template <typename... T>
using Action = std::function<void(T...)>;

template <typename TResult, typename... T>
using Func = std::function<TResult(T...)>;

}
