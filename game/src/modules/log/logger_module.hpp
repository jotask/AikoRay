#pragma once

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include "modules/module.hpp"
#include "utils/aiko_types.hpp"

namespace aiko
{

    class LoggerModule : public Module
    {
    public:
        virtual void init();

        static Ref<spdlog::logger>& GetCoreLogger()
        {
            return s_CoreLogger;
        }

        static Ref<spdlog::logger>& GetClientLogger()
        {
            return s_ClientLogger;
        }

    private:

        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;

    };

}
