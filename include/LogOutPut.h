#ifndef __LOGOUTPUT_H__
#define __LOGOUTPUT_H__

#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <iostream>
#include "utils.hpp"

/**
/*@brief 日志输出基类
*/
class LogOutPutBase
{
public:
    typedef std::shared_ptr<LogOutPutBase> ptrBase;
    virtual void log(const char *data, size_t length) = 0;
    /**
    /*@brief 虚析构函数，确保子类资源释放
    /*
     */
    virtual ~LogOutPutBase()
    {
    }

};

/*
/*@brief 日志输出到标准输出
 */
class LogOutPutStdout : public LogOutPutBase
{
public:
    void log(const char *data, size_t length) override;
};

/*
/*@brief 日志输出到指定文件中
 */
class LogOutPutFile : public LogOutPutBase
{
public:
    LogOutPutFile(const std::string &pathname);

    void log(const char *data, size_t length) override;

private:
    std::string m_pathName;
    std::ofstream m_ofsFile;
};

/*
/*@brief 日志输出到滚动文件中
/*
 */
class LogOutPutRollFile : public LogOutPutBase
{
public:
    LogOutPutRollFile(const std::string &fileName, size_t fileSize);
    void log(const char *data, size_t length) override;

    /*
    /*@brief 生成日志文件名
    /*
    /*@return std::string 生成文件名字
     */
    std::string MakeFileName();
private:
    std::string m_fileName; //文件名
    size_t m_fileMaxSize;      //文件大小
    size_t m_curFileSize;   //当前文件大小
    size_t m_countFiles;
    std::ofstream m_ofsFile;

};
/*
/*@brief 因为日志输出方式多样，此处使用工厂模式实现日志输出的可扩展
/*
 */
class LogOutPutFactory
{
public:
    /*此处采用模版，方便扩展*/
    template<class outPutType, class ...Args>
    static LogOutPutBase::ptrBase create(Args &&...args)
    {
        return std::make_shared<outPutType>(std::forward<Args>(args)...);
    }
};
#endif /* __LOGOUTPUT_H__ */