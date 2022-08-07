#pragma once

#include <filesystem>

#include "core/base.hpp"
#include "core/log.h"

#ifdef AIKO_ENABLE_ASSERTS

    // Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
    // provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
    #define AIKO_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { aiko##type##ERROR(msg, __VA_ARGS__); AIKO_DEBUGBREAK(); } }
    #define AIKO_INTERNAL_ASSERT_WITH_MSG(type, check, ...) AIKO_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
    #define AIKO_INTERNAL_ASSERT_NO_MSG(type, check) AIKO_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", AIKO_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

    #define AIKO_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
    #define AIKO_INTERNAL_ASSERT_GET_MACRO(...) AIKO_EXPAND_MACRO( AIKO_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, AIKO_INTERNAL_ASSERT_WITH_MSG, AIKO_INTERNAL_ASSERT_NO_MSG) )

    // Currently accepts at least the condition and one additional parameter (the message) being optional
    #define AIKO_ASSERT(...) AIKO_EXPAND_MACRO( AIKO_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
    #define AIKO_CORE_ASSERT(...) AIKO_EXPAND_MACRO( AIKO_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
    #define AIKO_ASSERT(...)
    #define AIKO_CORE_ASSERT(...)
#endif
