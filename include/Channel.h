#pragma once

#include <functional>
#include <memory>
#include <sys/epoll.h>

#include "noncopyable.h"
#include "timestamp.h"

class EventLoop;

/*
 * EventLoop, Channel, Poller之间的关系 对应多路事件分发器
 * Channel理解为通道 Channel在muduo中封装了sockfd和其感兴趣的event,如EPOLLIN,EPOLLOUT事件
 * 还绑定了poller返回的具体事件
 */
class Channel : noncopyable
{
public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(TimeStamp)>;  

    Channel(EventLoop* loop, int fd);
    ~Channel();

    // fd得到poller通知以后,处理事件的。调用相应的回调方法
    void handleEvent(TimeStamp receiveTime);

    // 设置回调函数对象
    void setReadcallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
    void setWritecallback(EventCallback cb) { writeCallback_ = std::move(cb); }
    void setClosecallback(EventCallback cb) { closeCallback_ = std::move(cb); }
    void setErrorcallback(EventCallback cb) { errorCallback_ = std::move(cb); }

    // 防止当channel被手动remove掉,channel还在执行回调操作
    void tie(const std::shared_ptr<void>&);

    int fd() const { return fd_; }
    int events() const { return events_; }
    void set_revents(int revt) { revents_ = revt; }

    // 返回fd当前的事件状态
    bool isNoneEvent() const { return events_ == kNoneEvent; }
    bool isReading() const { return events_ & kReadEvent; }
    bool isWriting() const { return events_ & kWriteEvent; }

    // 设置fd相应的事件状态
    void enableReading() { events_ |= kReadEvent; update(); }
    void disableReading() { events_ &= ~kReadEvent; update(); }
    void enableWriting() { events_ |= kWriteEvent; update(); }
    void disableWriting() { events_ &= ~kWriteEvent; update(); }
    void disableAll() { events_ = kNoneEvent; update(); }

    int index() const { return index_; }
    void setIndex(int idx) { index_ = idx; }

    EventLoop* ownerLoop() const { return loop_; }
    void remove();

private:
    void update();
    void handleEventWithGuard(TimeStamp receiveTime);

    static constexpr int kNoneEvent = 0;
    static constexpr int kReadEvent = EPOLLIN | EPOLLPRI;
    static constexpr int kWriteEvent = EPOLLOUT; 

    EventLoop* loop_; // 事件循环
    const int fd_;    // poller监听的对象
    int events_;      // 注册fd感兴趣的时间
    int revents_;     // poller返回的具体发生的事件
    int index_;

    std::weak_ptr<void> tie_;
    bool tied_;
    
    // 因为Channel通道里面能够获知fd最终发生的具体事件revents,所以它负责调用具体事件的回调操作
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};