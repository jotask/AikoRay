#pragma once

#include <memory>

#include "core/platform_detection.hpp"

#ifdef AIKO_DEBUG
	#if defined(AIKO_PLATFORM_WINDOWS)
		#define AIKO_DEBUGBREAK() __debugbreak()
	#elif defined(AIKO_PLATFORM_LINUX)
		#include <signal.h>
		#define AIKO_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	// TODO FIXME Fix enabling asserts. Currently macro expand to lower case
	// #define AIKO_ENABLE_ASSERTS
#else
	#define AIKO_DEBUGBREAK()
#endif

#define AIKO_EXPAND_MACRO(x) x
#define AIKO_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define AIKO_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }