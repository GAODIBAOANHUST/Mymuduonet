#include "Logger.h"
#include "timestamp.h"

#include <iostream>

// 获取日志唯一的实例对象
Logger &Logger::getinstance()
{
    static Logger logger;
    return logger;
}

// 设置日志级别
void Logger::setLogLevel(LogLevel level)
{
    loglevel_ = level;
}

// 写日志   [级别信息] time : msg
void Logger::log(std::string msg)
{
    switch (loglevel_)
    {
    case LogLevel::INFO:
        std::cout << "[INFO]";
        break;
    case LogLevel::ERROR:
        std::cout << "[ERROR]";
        break;
    case LogLevel::FATAL:
        std::cout << "[DEBUG]";
        break;
    case LogLevel::DEBUG:
        std::cout << "[DEBUG]";
        break;
    default:
        break;
    }

    // 打印时间和msg
    std::cout << TimeStamp::now().toString() << " : " << msg << std::endl;
}