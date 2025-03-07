#ifndef __BUFFER_H__
#define __BUFFER_H__

/*
/*@brief 一个用于中间存放日志消息的 buffer
/*
 */

#include <iostream>
#include  <vector>
#include <assert.h>

#define DEFAULT_BUFFER_SIZE 1 * 1024 * 1024 //缓冲区默认大小为 1M

#define MAX_BUFFER_SIZE 5 * 1024 * 1024 //缓冲区最大为 5M

#define BUFFER_INCREAMENT_STEP 1 * 1024 * 1024 //缓冲区超过最大值后，每次扩容的步长

class Buffer
{
public:
    Buffer(int bufferSize = DEFAULT_BUFFER_SIZE) : m_buffer(bufferSize), m_writeIndex(0), m_readIndex(0)
    {

    }
    
    /*
    /*@brief 日志写入缓冲区
    /*
    /*@param data 内容
    /*@param length 长度
     */
    inline void writeData(const char *data, int length)
    {
        if (GetWritableSize() < length)
        {
            Reserve(length);
        }
        std::copy(data, data + length, &m_buffer[m_writeIndex]);
        MoveWriteIndex(length);
    }

    /*
    /*@brief 移动读索引位置
    /*
    /*@param length 移动长度
     */
    void MoveReadIndex(int length)
    {
        m_readIndex = GetReadableSize() < length ? GetReadableSize() : m_readIndex + length;
    }

    /*
    /*@brief 移动写索引位置
    /*
    /*@param length 
     */
    void MoveWriteIndex(int length)
    {
        assert(m_writeIndex + length <= m_buffer.size());
        m_writeIndex += length;
    }
    /*
    /*@brief 获取可写缓冲区大小
    /*
    /*@return size_t 缓冲区大小
     */
    size_t GetWritableSize()
    {
        return m_buffer.size() - m_writeIndex;
    }

    /*
    /*@brief 返回可读空间大小
    /*
    /*@return size_t 空间大小
     */
    size_t GetReadableSize()
    {
         return m_writeIndex - m_readIndex;
    }

    /*
    /*@brief 返回可读缓冲区指针
    /*
    /*@return const char* 
     */
    const char *GetReadalePtr()
    {
        return &m_buffer[m_readIndex];
    }

    void reset()
    {
        m_readIndex = 0;
        m_writeIndex = 0;
    }

    void swap(Buffer &buffer)
    {
        std::swap(m_writeIndex, buffer.m_writeIndex);
        std::swap(m_readIndex, buffer.m_readIndex);
        m_buffer.swap(buffer.m_buffer);
    }

    bool IsEmpty()
    {
        return m_writeIndex == m_readIndex;
    }
    /*
    /*@brief 扩容缓冲区
    /*
    /*@param length 
     */
    void Reserve(int length)
    {
        int newSize = 0;
        if (m_buffer.size() < MAX_BUFFER_SIZE)
        {
            newSize = m_buffer.size() * 2 + length;
        }
        else
        {
            newSize = m_buffer.size() + BUFFER_INCREAMENT_STEP + length;
        }

        m_buffer.resize(newSize);
    }
private:
    std::vector<char> m_buffer; //用于存放日志的缓冲区，vector 方便扩容
    int m_writeIndex; //写索引
    int m_readIndex; //读索引
};
#endif /* __BUFFER_H__ */