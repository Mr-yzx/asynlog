#ifndef __LOGMSG_H__
#define __LOGMSG_H__

/*
/*@brief 日志消息类，包含日志输出的格式，日志输出格式如下
/*   时间戳 + 日志级别 + 日志器 + 所属线程 id + 文件名 + 行号 + 具体内容
 */
#include <string>
#include <ctime>
#include <thread>
#include "utils.hpp"
#include "LogLevel.hpp"

/*
/*@brief 日志结构体
/*
 */
struct LogMsg
{
    time_t m_time;
    LogLevel::Level m_level;
    std::string m_loggerName;
    std::thread::id m_threadID;
    std::string m_fileName;  //文件名
    int m_lineNum;
    std::string m_data;

    LogMsg()
    {

    }

    LogMsg(LogLevel::Level level, std::string loggerName, std::string fileName, int lineNum, std::string data)
    {
        m_time = DateOperation::GetTimeStamp();
        m_level = level;
        m_loggerName = loggerName;
        m_threadID = std::this_thread::get_id();
        m_fileName = fileName;
        m_lineNum = lineNum;
        m_data = data;
    }
};
#endif /* __LOGMSG_H__ */