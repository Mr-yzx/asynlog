#ifndef __LOG_H__
#define __LOG_H__
/*
/*@brief 日志库头文件
/*
 */
#include "Logger.h"


#define Info(format, ...) LogInfo(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define Warning(format, ...) LogWarning(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define Debug(format, ...) LogDebug(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define Error(format, ...) LogError(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define Fatal(format, ...) LogFatal(__FILE__, __LINE__, format, ##__VA_ARGS__)


/*一个全局接口，获取指定日志器*/
const Logger::ptr &GetLogger(std::string name);

/*一个全局接口，获取默认日志器*/
const Logger::ptr &GetDefaultLogger();


#endif /* __LOG_H__ */