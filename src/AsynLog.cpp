#include "AsynLog.h"

void AsynLog::WriteData(const char *data, int length)
{
    std::unique_lock<std::mutex> lock(m_mutex); //加锁
    if (m_type == ASYNLOG_TYPE::ASYNLOG_TYPE_SAFE)
    {
        /*阻塞，直到生产者缓冲区足够写入*/
        m_condProducer.wait(lock, [&](){
            return m_buffProducer.GetWritableSize() >= length;
        });
    }
    m_buffProducer.writeData(data, length);
    m_condConsumer.notify_one(); //通知消费者
}

void AsynLog::stop()
{
    m_isStop = true;
    m_condConsumer.notify_all();
    m_threadID.join();
}

void AsynLog::AsynThreadEntry()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        /*消费者阻塞直到有数据可读*/
        m_condConsumer.wait(lock, [&](){
            return m_isStop || m_buffProducer.GetReadableSize() > 0;
        });

        if (m_isStop && m_buffProducer.IsEmpty())
        {
            break;
        }

        if (m_type == ASYNLOG_TYPE::ASYNLOG_TYPE_SAFE)
        {
            
            m_condProducer.notify_all(); //通知生产者
        }
        m_cb(m_bufferConsumer);
        m_bufferConsumer.reset();
    }
}
