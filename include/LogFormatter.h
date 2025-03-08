#ifndef __LOGFORMATTER_H__
#define __LOGFORMATTER_H__

/*
/*@brief 格式化日志
/*
 */

#include <vector>
#include <assert.h>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <memory>
#include <ctime>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "LogMsg.h"

/*
/*@brief 格式化基类
/*
 */
class LogFormatterItemBase
{
public:
    typedef std::shared_ptr<LogFormatterItemBase> ptr;
    virtual void Format(std::ostream &ofs, LogMsg &msg) = 0;
};

/*
/*@brief 日期格式化
/*
 */
class DateFormatterItem : public LogFormatterItemBase
{
public:
        DateFormatterItem(const std::string &format = "%H:%M:%S") : m_dateFormat(format.size() == 0 ? "%H:%M:%S" : format)
        {

        }
        /*重写父类函数*/
        void Format(std::ostream &ofs, LogMsg &msg) override
        {
            struct tm t;
            char buff[32] = {0};
            localtime_r(&msg.m_time, &t);
            strftime(buff, sizeof(buff), m_dateFormat.c_str(), &t);
            ofs << buff;
        }
    private:
        std::string m_dateFormat;
};

/*
/*@brief 日志等级格式化
/*
 */
class LogLevelFormatterItem : public LogFormatterItemBase
{
public:
    void Format(std::ostream &ofs, LogMsg &msg) override
    {
        ofs << LogLevel::LogLevelToString(msg.m_level);
    }

};

/*
/*@brief 日志器名称格式化
/*
 */
class LoggerFormatterItem : public LogFormatterItemBase
{
public:
    void Format(std::ostream &ofs, LogMsg &msg) override
    {
        ofs << msg.m_loggerName;
    }
};

/*
/*@brief 线程相关格式化
/*
 */
class ThreadFormatterItem : public LogFormatterItemBase
{
public:
    void Format(std::ostream &ofs, LogMsg &msg) override
    {
        ofs << msg.m_threadID;
    }
};

/*
/*@brief 文件相关格式化
/*
 */
class FileFormatterItem : public LogFormatterItemBase
{
public:
    void Format(std::ostream &ofs, LogMsg &msg) override
    {
        ofs << msg.m_fileName;
    }
};

class LineFormatterItem : public LogFormatterItemBase
{
public:
    void Format(std::ostream &ofs, LogMsg &msg) override
    {
        ofs << msg.m_lineNum;
    }
};

/*
/*@brief 具体日志消息格式化
/*
 */
class MsgFormatterItem : public LogFormatterItemBase
{
public:
    void Format(std::ostream &ofs, LogMsg &msg) override
    {
        ofs << msg.m_data;
    }
};

/*
/*@brief 日志格式化器
/*
 */
class LogFormatter
{
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string &formatString = "[%d{%H:%M:%S}][%p][%c][%t][%f:%l][%m]") : m_formatString(formatString)
    {
    
    }
    void Format(std::ostream &ofs, LogMsg &msg);

    std::string Format(LogMsg &msg)
    {
        std::stringstream ss;
        Format(ss, msg);
        return ss.str();

        
    }

    bool ParseFormatString();

    LogFormatterItemBase::ptr CreateFormatItem(const std::string &key, const std::string &val);
private:
    std::string m_formatString;
    std::vector<LogFormatterItemBase::ptr> m_itemBase;
};
#endif /* __LOGFORMATTER_H__ */