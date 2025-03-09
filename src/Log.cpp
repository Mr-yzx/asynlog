#include "Log.h"

const Logger::ptr &GetLogger(std::string name)
{
    return LoggerManager::GetInstance().GetSureLogger(name);
}

const Logger::ptr &GetDefaultLogger()
{
    return LoggerManager::GetInstance().GetDefaultLogger();
}