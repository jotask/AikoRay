#pragma once

#include "modules/log/logger_module.hpp"

// Core log macros
#define AIKO_CORE_TRACE(...)    ::aiko::LoggerModule::GetCoreLogger()->trace(__VA_ARGS__)
#define AIKO_CORE_INFO(...)     ::aiko::LoggerModule::GetCoreLogger()->info(__VA_ARGS__)
#define AIKO_CORE_WARN(...)     ::aiko::LoggerModule::GetCoreLogger()->warn(__VA_ARGS__)
#define AIKO_CORE_ERROR(...)    ::aiko::LoggerModule::GetCoreLogger()->error(__VA_ARGS__)
#define AIKO_CORE_CRITICAL(...) ::aiko::LoggerModule::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define AIKO_TRACE(...)         ::aiko::LoggerModule::GetClientLogger()->trace(__VA_ARGS__)
#define AIKO_INFO(...)          ::aiko::LoggerModule::GetClientLogger()->info(__VA_ARGS__)
#define AIKO_WARN(...)          ::aiko::LoggerModule::GetClientLogger()->warn(__VA_ARGS__)
#define AIKO_ERROR(...)         ::aiko::LoggerModule::GetClientLogger()->error(__VA_ARGS__)
#define AIKO_CRITICAL(...)      ::aiko::LoggerModule::GetClientLogger()->critical(__VA_ARGS__)
