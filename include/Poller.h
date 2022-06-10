#pragma once

#include <vector>
#include <unordered_map>

#include "noncopyable.h"
#include "timestamp.h"

class Channel;
class EventLoop;

// muduo库中多路事件分发器的核心IO复用模块
class Poller : noncopyable
{
public:
    using ChannelList = std::vector<Channel*>;

    Poller(EventLoop* loop);
    virtual ~Poller() = default;

    // 给所有IO复用保留统一的接口
    virtual TimeStamp poll(int timeoutMs, ChannelList* activeChannels) = 0; // 启动epoll_wait
    virtual void updateChannel(Channel* channel) = 0; // epoll_ctl
    virtual void removeChannel(Channel* channel) = 0; // epoll_ctl

    // 判断参数Channel是否在当前Poller中
    bool hasChannel(Channel* channel) const;

    // 类似单例模式,EventLoop可以通过该接口获取默认的IO复用的具体实现
    static Poller* newDefaultPoller(EventLoop* loop);
protected:
    // map的key: sockfd    value: sockfd所属的channel通道类型
    using ChannelMap = std::unordered_map<int, Channel*>;
    ChannelMap channels_;
private:
    EventLoop* ownerLoop_; // 定义Poller所属的事件循环EventLoop
};