#ifndef __ASYNLOG_H__
#define __ASYNLOG_H__

/*
/*@brief 异步日志类
/*
 */

#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <functional>
#include <string>
#include <memory>
#include "Buffer.hpp"
enum class ASYNLOG_TYPE
{
    ASYNLOG_TYPE_SAFE = 0,
    ASYNLOG_TYPE_UNSAFE
};
class AsynLog
{
public:
    typedef std::function<void(Buffer &)> call_back;
    typedef std::shared_ptr<AsynLog> ptr;

    AsynLog(call_back cb, ASYNLOG_TYPE type = ASYNLOG_TYPE::ASYNLOG_TYPE_SAFE)
            : m_type(type), m_isStop(false), m_cb(cb), m_threadID(&AsynLog::AsynThreadEntry, this)
    {

    }
    ~AsynLog()
    {
        stop();
    }
    /*生产者向缓冲区写数据*/
    void WriteData(const char *data, int length);

    /*停止线程*/
    void stop();



private:

    /*异步线程入口函数*/
    void AsynThreadEntry();

private:
    ASYNLOG_TYPE m_type;
    std::atomic<bool> m_isStop;
    std::mutex m_mutex;
    std::condition_variable m_condProducer;
    std::condition_variable m_condConsumer; ;
    std::thread m_threadID;
    Buffer m_buffProducer;
    Buffer m_bufferConsumer;
    call_back m_cb;
};
#endif /* __ASYNLOG_H__ */