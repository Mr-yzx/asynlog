#include "LogFormatter.h"

void LogFormatter::Format(std::ostream &ofs, LogMsg &msg)
{
    for (auto &item : m_itemBase)
    {
        item->Format(ofs, msg);
    }
}

bool LogFormatter::ParseFormatString()
{
    std::unordered_set<char> format = { 'd', 'p', 'c', 't', 'f', 'l', 'm'};
    std::vector<std::pair<std::string, std::string>> formatVec;

    std::string key, val;

    int pos;
    while (pos < m_formatString.size())
    {
        if (m_formatString[pos] != '%')
        {
            val.push_back(m_formatString[pos++]);
            continue;;
        }
        if (pos + 1 < m_formatString.size() && m_formatString[pos + 1] == '%')
        {
            val.push_back('%');
            pos += 2;
            continue;
        }
        if (!val.empty())
        {
            formatVec.push_back(std::make_pair("", val));
            val.clear();
        }
        if (pos + 1 >= m_formatString.size())
        {
            std::cerr << "已经是最后一个%" << std::endl;
            return false;
        }
        else
        {
            auto ite = format.find(m_formatString[pos + 1]);
            if (ite != format.end())
            {
                key.push_back(m_formatString[pos + 1]);
                pos += 2;
            }
            else
            {
                std::cerr << "格式化字符错误" << std::endl;
                return false;
            }
        }
        if (pos < m_formatString.size() && m_formatString[pos] == '{')
        {
            int endpos = m_formatString.find_last_of('}', pos + 1);
            if (endpos == std::string::npos)
            {
                std::cerr << "未找到匹配的{}字符" << std::endl;
                return false;
            }
            else
            {
                val += m_formatString.substr(pos + 1, endpos - pos - 1);
                pos = endpos + 1;
            }
        }
        formatVec.push_back(std::make_pair(key,val));
        key.clear();
        val.clear();
    }
    for (auto &item : formatVec)
    {
        m_itemBase.push_back(CreateFormatItem(item.first, item.second));
    }
    return true;
}

LogFormatterItemBase::ptr LogFormatter::CreateFormatItem(const std::string &key, const std::string &val)
{
    if (key == "d")
    {
        return std::make_shared<DateFormatterItem>(val);
    }
    else if (key == "p")
    {
        return std::make_shared<LogLevelFormatterItem>();
    }
    else if (key == "c")
    {
        return std::make_shared<LoggerFormatterItem>();
    }
    else if (key == "t")
    {
        return std::make_shared<ThreadFormatterItem>();
    }
    else if (key == "f")
    {
        return std::make_shared<FileFormatterItem>();
    }
    else if (key == "l")
    {
        return std::make_shared<LineFormatterItem>();
    }
    else if (key == "m")
    {
        return std::make_shared<MsgFormatterItem>();
    }
    else
    {
        std::cerr << "未知的格式化字符" << std::endl;
        return nullptr;
    }
}
