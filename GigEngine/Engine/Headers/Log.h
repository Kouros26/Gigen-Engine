#pragma once

#include <iostream>

#define GIG_LOG(message) GigDebug::Logger::Log(message)
#define GIG_LOGWARNING(message) GigDebug::Logger::LogWarning(message)
#define GIG_LOGERROR(message) GigDebug::Logger::LogError(message)

namespace GigDebug
{
    class Logger
    {
    public:
        Logger() = default;

        static void Log(const std::string& pMessage);
        static void LogWarning(const std::string& pMessage);
        static void LogError(const std::string& pMessage);
    };
}
