#ifndef __LOGLEVEL_H__
#define __LOGLEVEL_H__

/*
/*@brief 日志级别类
/*
 */

#include <string>
#include <iostream>

class LogLevel
{
public:
    enum class Level
    {
        DEBUG = 0,
        INTO,
        WARNING,
        ERROR,
        FATAL,
        OFF
    };
    std::string LogLevelToString(LogLevel::Level level)
    {
        switch (level)
        {
            case LogLevel::Level::DEBUG:
                return "DEBUG";
            case LogLevel::Level::INTO:
                return "INFO";
            case LogLevel::Level::WARNING:
                return "WARNING";
            case LogLevel::Level::ERROR:
                return "ERROR";
            case LogLevel::Level::FATAL:
                return "FATAL";
            case LogLevel::Level::OFF:
                return "OFF";
            default:
                break;
        }
        return "UNKNOWN";
    }
};
#endif /* __LOGLEVEL_H__ */