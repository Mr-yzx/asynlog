#include "Logger.h"

void Logger::LogInfo(const std::string &filename, int linenum, const char *format, ...)
{
    if (LogLevel::Level::INFO < m_levelMax)
    {
        return;
    }
    va_list ap;
    va_start(ap, format);
    char *data = NULL;
    if (vasprintf(&data, format, ap) == -1)
    {
        return;
    }
    va_end(ap);

    LogMsg msg(LogLevel::Level::INFO, m_loggerName, filename, linenum, std::string(data));
    std::string logmsg = m_formatPtr->Format(msg);
    log(logmsg.c_str(), logmsg.size());
    free(data);
}

void Logger::LogWarning(const std::string &filename, int linenum, const char *format, ...)
{
    if (LogLevel::Level::WARNING < m_levelMax)
    {
        return;
    }
    va_list ap;
    va_start(ap, format);
    char *data = NULL;
    if (vasprintf(&data, format, ap) == -1)
    {
        return;
    }
    va_end(ap);

    LogMsg msg(LogLevel::Level::WARNING, m_loggerName, filename, linenum, std::string(data));
    std::string logmsg = m_formatPtr->Format(msg);
    log(logmsg.c_str(), logmsg.size());
    free(data);
}

void Logger::LogDebug(const std::string &filename, int linenum, const char *format, ...)
{
    if (LogLevel::Level::DEBUG < m_levelMax)
    {
        return;
    }
    va_list ap;
    va_start(ap, format);
    char *data = NULL;
    if (vasprintf(&data, format, ap) == -1)
    {
        return;
    }
    va_end(ap);

    LogMsg msg(LogLevel::Level::DEBUG, m_loggerName, filename, linenum, std::string(data));
    std::string logmsg = m_formatPtr->Format(msg);
    log(logmsg.c_str(), logmsg.size());
    free(data);
}

void Logger::LogError(const std::string &filename, int linenum, const char *format, ...)
{
    if (LogLevel::Level::ERROR < m_levelMax)
    {
        return;
    }
    va_list ap;
    va_start(ap, format);
    char *data = NULL;
    if (vasprintf(&data, format, ap) == -1)
    {
        return;
    }
    va_end(ap);

    LogMsg msg(LogLevel::Level::ERROR, m_loggerName, filename, linenum, std::string(data));
    std::string logmsg = m_formatPtr->Format(msg);
    log(logmsg.c_str(), logmsg.size());
    free(data);
}

void Logger::LogFatal(const std::string &filename, int linenum, const char *format, ...)
{
    if (LogLevel::Level::FATAL < m_levelMax)
    {
        return;
    }
    va_list ap;
    va_start(ap, format);
    char *data = NULL;
    if (vasprintf(&data, format, ap) == -1)
    {
        return;
    }
    va_end(ap);

    LogMsg msg(LogLevel::Level::FATAL, m_loggerName, filename, linenum, std::string(data));
    std::string logmsg = m_formatPtr->Format(msg);
    log(logmsg.c_str(), logmsg.size());
    free(data);
}

void SyncLogger::log(const char *data, int length)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    for (auto &out : m_outVec)
    {
        if (out.get() != nullptr)
        {
            out->log(data, length);
        }
    }
}

void AsynLogger::DataOutput(Buffer &buff)
{
    for (auto &out : m_outVec)
    {
        if (out.get() != nullptr)
        {
            out->log(buff.GetReadalePtr(), buff.GetReadableSize());
        }
    }
}

Logger::ptr LocalLoggerBuild::BuildLogger()
{
    assert(!m_loggerNmae.empty());
    if (m_formatPtr.get() == nullptr)
    {
        m_formatPtr = std::make_shared<LogFormatter>();
    }

    if (m_outputVec.empty())
    {
        m_outputVec.push_back(LogOutPutFactory::create<LogOutPutStdout>());

    }

    if (m_loggerType == LoggerType::ASYN_LOGGER)
    {
        return std::make_shared<AsynLogger>(m_loggerNmae, m_levelMax, m_formatPtr, m_outputVec, m_asynType);

    }
    return std::make_shared<SyncLogger>(m_loggerNmae, m_levelMax, m_formatPtr, m_outputVec);
}

Logger::ptr GlobalLoggerBuild::BuildLogger()
{
    assert(!m_loggerNmae.empty());
    if (m_formatPtr.get() == nullptr)
    {
        m_formatPtr = std::make_shared<LogFormatter>();

    }

    if (m_outputVec.empty())
    {
        m_outputVec.push_back(LogOutPutFactory::create<LogOutPutStdout>());
    }

    Logger::ptr logger;
    if (m_loggerType == LoggerType::ASYN_LOGGER)
    {
        logger = std::make_shared<AsynLogger>(m_loggerNmae, m_levelMax, m_formatPtr, m_outputVec, m_asynType);
    }
    else
    {
        logger = std::make_shared<SyncLogger>(m_loggerNmae, m_levelMax, m_formatPtr, m_outputVec);
    }

    LoggerManager::GetInstance().AddLogger(logger);
    return logger;
}
