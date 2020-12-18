#pragma once

#include <memory>

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

#include "Utilities/Log.h"

#ifdef _DEBUG
	#define HP_ASSERT(x, ...) { if(!(x)) { HP_FATAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HP_ASSERT(x, ...)
#endif