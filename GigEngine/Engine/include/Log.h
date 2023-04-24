#pragma once

#include <iostream>

#define GIG_LOG(message) GigDebug::Logger::Log(message)

namespace GigDebug
{
    class Logger
    {
    public:
        Logger() = default;

        static void Log(const std::string& pMessage);
    };
}
