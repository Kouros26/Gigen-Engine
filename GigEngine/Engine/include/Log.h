#pragma once

#include <iostream>

#define GIG_LOG(message) GigDebug::Logger::Log(pMessage)

namespace GigDebug
{
    class Logger
    {
    public:
        Logger() = default;

        static void Log(const std::string& pMessage);
    };
}
