#pragma once

#include <string>
#include <functional>

#include "noncopyable.h"

// 定义日志的级别   INFO,ERROR,FATAL,DEBUG
enum class LogLevel
{
    INFO,  //普通信息
    ERROR, //错误信息
    FATAL, //core信息
    DEBUG, //调试信息
};

// 为什么在课程中使用宏定义?
// 用户只需要调用函数,不需要再自己创建类了

// 使用宏定义
#define LOG_INFO(LogmsgFormat,...) \
    do \
    { \
        Logger& logger = Logger::getinstance(); \
        logger.setLogLevel(LogLevel::INFO); \
        char buf[1024]{0}; \
        snprintf(buf, 1024, LogmsgFormat, ##__VA_ARGS__); \
        logger.log(buf); \
    } while(0)

#define LOG_ERROR(LogmsgFormat,...) \
    do \
    { \
        Logger& logger = Logger::getinstance(); \
        logger.setLogLevel(LogLevel::ERROR); \
        char buf[1024]{0}; \
        snprintf(buf, 1024, LogmsgFormat, ##__VA_ARGS__); \
        logger.log(buf); \
    } while(0)

#define LOG_FATAL(LogmsgFormat,...) \
    do \
    { \
        Logger& logger = Logger::getinstance(); \
        logger.setLogLevel(LogLevel::FATAL); \
        char buf[1024]{0}; \
        snprintf(buf, 1024, LogmsgFormat, ##__VA_ARGS__); \
        logger.log(buf); \
        exit(-1); \
    } while(0)

#ifdef MUDEBUG
#define LOG_DEBUG(LogmsgFormat,...) \
    do \
    { \
        Logger& logger = Logger::getinstance(); \
        logger.setLogLevel(LogLevel::DEBUG); \
        char buf[1024]{0}; \
        snprintf(buf, 1024, LogmsgFormat, ##__VA_ARGS__); \
        logger.log(buf); \
    } while(0)
#else
#define LOG_DEBUG(LogmsgFormat,...)
#endif

// 输出一个日志类
class Logger : noncopyable
{
public:
    // 获取日志唯一的实例对象
    static Logger& getinstance();
    // 设置日志级别
    void setLogLevel(LogLevel );
    // 写日志
    void log(std::string );
private:
    LogLevel loglevel_;
    Logger() {}
};