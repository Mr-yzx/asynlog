#include  <assert.h>
#include "LogOutPut.h"

void LogOutPutStdout::log(const char *data, size_t length)
{
    std::cout.write(data, length);
}

LogOutPutFile::LogOutPutFile(const std::string &pathname) : m_pathName(pathname)
{
    /*路径不存在则创建*/
    if (!FileOperation::IsPathExist(FileOperation::GetFilePath(m_pathName)))
    {
        FileOperation::CreateDirectory(FileOperation::GetFilePath(m_pathName));
    }

    /*创建文件并打开*/
    m_ofsFile.open(m_pathName, std::ios::binary | std::ios::app);
    if (!m_ofsFile.is_open())
    {
        std::cerr << "打开日志输出文件失败" << std::endl;
        abort();
    }
}

void LogOutPutFile::log(const char *data, size_t length)
{
    m_ofsFile.write(data, length);
}

LogOutPutRollFile::LogOutPutRollFile(const std::string &fileName, size_t fileSize)
        : m_fileName(fileName), m_fileMaxSize(fileSize), m_curFileSize(0), m_countFiles(0)
{
    /*路径不存在则创建*/
    if (!FileOperation::IsPathExist(FileOperation::GetFilePath(m_fileName)))
    {
        FileOperation::CreateDirectory(FileOperation::GetFilePath(m_fileName));
    }

    /*创建 rollfile 文件*/
    std::string newfile = MakeFileName();
    m_ofsFile.open(newfile, std::ios::binary | std::ios::app);
    assert(m_ofsFile.is_open());

}

std::string LogOutPutRollFile::MakeFileName()
{
    struct tm tmp_time = DateOperation::GetTimeNow();
    std::string file_name = m_fileName + "-" + std::to_string(tmp_time.tm_year + 1900) + std::to_string(tmp_time.tm_mon + 1) + std::to_string(tmp_time.tm_mday)
                            + "-" + std::to_string(tmp_time.tm_hour) + std::to_string(tmp_time.tm_min) + std::to_string(tmp_time.tm_sec) + ".log";
    return file_name;
}
void LogOutPutRollFile::log(const char *data, size_t length)
{
    if (m_curFileSize >= m_fileMaxSize)
    {
        m_ofsFile.close();

        /*新生成一个文件*/
        std::string newfileName = MakeFileName();
        m_ofsFile.open(newfileName, std::ios::binary | std::ios::app);
        assert(m_ofsFile.is_open());
        m_curFileSize = 0; //重置文件大小
    }
    m_ofsFile.write(data, length);
    m_curFileSize += length;
}

