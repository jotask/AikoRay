#include "logger_module.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace aiko
{

    Ref<spdlog::logger> LoggerModule::s_CoreLogger;
    Ref<spdlog::logger> LoggerModule::s_ClientLogger;

    void LoggerModule::init()
    {

        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("aiko.log", true));

        logSinks[0]->set_pattern("%^[%T] %n: %v%$");
        logSinks[1]->set_pattern("[%T] [%l] %n: %v");

        s_CoreLogger = std::make_shared<spdlog::logger>("aiko", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_CoreLogger);
        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger->flush_on(spdlog::level::trace);

        s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_ClientLogger);
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger->flush_on(spdlog::level::trace);

        // TODO
        /*
        auto customLogger = [](int msgType, const char* text, va_list args)
        {
            char timeStr[64] = { 0 };
            time_t now = time(NULL);
            struct tm* tm_info = localtime(&now);

            strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
            printf("[%s] ", timeStr);

            switch (msgType)
            {
            case LOG_INFO: printf("[INFO] : "); break;
            case LOG_ERROR: printf("[ERROR]: "); break;
            case LOG_WARNING: printf("[WARN] : "); break;
            case LOG_DEBUG: printf("[DEBUG]: "); break;
            default: break;
            }

            vprintf(text, args);
            printf("\n");
        };
        SetTraceLogCallback(customLogger);
        */

    }

}
