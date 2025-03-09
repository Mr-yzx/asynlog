#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <vector>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <cstdarg>
#include "LogLevel.hpp"
#include "LogFormatter.h"
#include "LogOutPut.h"
#include "AsynLog.h"


/*
/*@brief 日志器模块，日志消息模块封装，完成不同的输出方式
/*
 */


/*日志器类型*/
enum class LoggerType
{
    SYNC_LOGGER = 0,
    ASYN_LOGGER
};

class Logger
{
public:
    typedef std::shared_ptr<Logger> ptr;
    Logger(const std::string logger, LogLevel::Level level, LogFormatter::ptr formatter, std::vector<LogOutPutBase::ptrBase> output)
        : m_loggerName(logger), m_levelMax(level), m_formatPtr(formatter), m_outVec(output)
    {

    }

    /*
    /*@brief 以不同等级输出日志
    /*
     */

    void LogInfo(const std::string &filename, int linenum, const char *format, ...);
    void LogWarning(const std::string &filename, int linenum, const char *format, ...);
    void LogDebug(const std::string &filename, int linenum, const char *format, ...);
    void LogError(const std::string &filename, int linenum, const char *format, ...);
    void LogFatal(const std::string &filename, int linenum, const char *format, ...);

    const std::string &GetLoggerName()
    {
        return m_loggerName;
    }

protected:
    virtual void log(const char *data, int length) = 0; //纯虚函数，子类实现
protected:
    std::mutex m_mutex;
    std::string m_loggerName;
    LogLevel::Level m_levelMax;
    LogFormatter::ptr m_formatPtr;
    std::vector<LogOutPutBase::ptrBase> m_outVec;
};

/*
/*@brief 同步日志器
/*
 */
class SyncLogger : public Logger
{
public:
    SyncLogger(const std::string logger, LogLevel::Level level, LogFormatter::ptr formatter, std::vector<LogOutPutBase::ptrBase> output)
        : Logger(logger, level, formatter, output)
    {

    }
protected:
    void log(const char *data, int length) override;
};

/*
/*@brief 异步日志器
/*
 */
class AsynLogger : public Logger
{
public:
    AsynLogger(const std::string logger, LogLevel::Level level, LogFormatter::ptr formatter, std::vector<LogOutPutBase::ptrBase> output, ASYNLOG_TYPE type)
        : Logger(logger, level, formatter, output), m_asynLog(std::bind(&AsynLogger::DataOutput, this, std::placeholders::_1), type)
    {

    }

protected:
    void log(const char *data, int length) override
    {
        m_asynLog.WriteData(data, length);
    }

    /*异步线程调用此函数，实现数据输出*/
    void DataOutput(Buffer &buff);
protected:
    AsynLog m_asynLog;
};

/*
/*@brief 使用建造者模式构建日志器，根据不同类型决定构造同步或异步日志器
/*
 */
class LoggerBuild
{
public:
    LoggerBuild();

    void BuildUnSafe()
    {
        m_asynType = ASYNLOG_TYPE::ASYNLOG_TYPE_UNSAFE;
    }

    void BuildLoggerType(LoggerType type = LoggerType::SYNC_LOGGER)
    {
        m_loggerType = type;
    }

    void BuildLoggerName(const std::string &loggername)
    {
        m_loggerNmae = loggername;
    }

    void BuildLogLevelMax(LogLevel::Level levelmax)
    {
        m_levelMax = levelmax;
    }

    void BuildLogFormatter(const std::string &formatString = "[%d{%H:%M:%S}][%p][%c][%t][%f:%l][%m]")
    {
        m_formatPtr = std::make_shared<LogFormatter>(formatString);
    }

    /*日志输出方式模板函数*/
    template <class LogOutPutType, class... Args>
    void BuildLogOutput(Args &&...args)
    {
        LogOutPutBase::ptrBase ptr = LogOutPutFactory::create<LogOutPutType>(std::forward<Args>(args)...);
        m_outputVec.push_back(ptr);
    }

    virtual Logger::ptr BuildLogger() = 0;
protected:
    LoggerType m_loggerType;
    ASYNLOG_TYPE m_asynType;
    std::string m_loggerNmae;
    LogLevel::Level m_levelMax;
    LogFormatter::ptr m_formatPtr;
    std::vector<LogOutPutBase::ptrBase> m_outputVec;
};

class LocalLoggerBuild : public LoggerBuild
{
public:
    LocalLoggerBuild()
    {

    }
    Logger::ptr BuildLogger() override;
};

class GlobalLoggerBuild : public LoggerBuild
{
public:
    Logger::ptr BuildLogger() override;
};

class LoggerManager
{
public:
    static LoggerManager &GetInstance()
    {
        static LoggerManager instance;
        return instance;
    }

    void AddLogger(const Logger::ptr &logger)
    {
        if (IsLoggerExist(logger->GetLoggerName()))
        {
            return;
        }
        std::unique_lock<std::mutex> lock(m_mutex);
        m_loggerMap.insert(std::unordered_map<std::string, Logger::ptr>::value_type(logger->GetLoggerName(), logger));
    }

    bool IsLoggerExist(const std::string &logger)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_loggerMap.find(logger) != m_loggerMap.end();
    }

    const Logger::ptr &GetDefaultLogger()
    {
        return m_defaultLogger;
    }

    const Logger::ptr &GetSureLogger(const std::string &loggername)
    {
        return m_loggerMap[loggername];
    }
private:
    LoggerManager()
    {
        std::unique_ptr<LoggerBuild> build(new LocalLoggerBuild());
        build->BuildLoggerName("default_logger");
        m_defaultLogger = build->BuildLogger();
        AddLogger(m_defaultLogger);
    }

    LoggerManager(const LoggerManager &) = delete;
private:
    std::mutex m_mutex;
    Logger::ptr m_defaultLogger;
    std::unordered_map<std::string, Logger::ptr> m_loggerMap;
};
#endif /* __LOGGER_H__ */