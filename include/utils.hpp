#ifndef __UTILS_H__
#define __UTILS_H__

/*
/*@brief 一个工具函数类
/*
 */
#include <iostream>
#include <string>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>

/*
/*@brief 时间操作类
/*
 */
class DateOperation
{
public:
    /*获取时间戳*/
    static time_t GetTimeStamp()
    {
        return time(nullptr);
    }

    /*获取时间*/
    static struct tm GetTimeNow()
    {
        struct tm time_val;
        time_t ts = GetTimeStamp();
        localtime_r(&ts, &time_val);
        return time_val;
    }
};

/*
/*@brief 文件操作类
/*
 */
class FileOperation
{
public:
    /*判断路径是否存在*/
    static bool IsPathExist(const std::string &pathName)
    {
        struct stat filestat;
        if (stat(pathName.c_str(), &filestat) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /*获取文件所在路径*/
    static std::string GetFilePath(const std::string &fileName)
    {
        size_t pos = fileName.find_last_of("/\\");
        if (pos == std::string::npos)
        {
            return ".";
        }
        else
        {
            return fileName.substr(0, pos + 1);
        }
    }

    /*创建目录*/
    static void CreateDirectory(const std::string &pathName)
    {
        if (pathName.size() == 0)
        {
            return;
        }
        umask(0);
        size_t curpos = 0, pos = 0;
        std::string parentDir;
        while (curpos < pathName.size())
        {
            pos = pathName.find_first_of("/\\", curpos);
            /*获取父级路径*/
            parentDir = pathName.substr(0, pos);

            if ((parentDir.size() != 0) && (!IsPathExist(parentDir)))
            {
                mkdir(parentDir.c_str(), 0777);
            }

            if (pos == std::string::npos)
            {
                break;
            }
            curpos= pos + 1;
        }
    }

};
#endif /* __UTILS_H__ */