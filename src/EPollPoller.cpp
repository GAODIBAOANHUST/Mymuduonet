#include "EPollPoller.h"
#include "Logger.h"
#include "Channel.h"

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

// channel未添加到poller中
constexpr int kNew = -1; // channel的成员index_ = -1
// channel已添加到poller中
constexpr int kAdded = 1;
// channel已从poller中删除
constexpr int kDeleted = 2;

EPollPoller::EPollPoller(EventLoop *loop)
    : Poller(loop), epollfd_(::epoll_create1(EPOLL_CLOEXEC)), events_(kInitEventListSize)
{
    if (epollfd_ < 0)
    {
        LOG_FATAL("epoll_create error: %d", errno); // 打印完日志直接exit(-1)结束进程
    }
}

EPollPoller::~EPollPoller()
{
    ::close(epollfd_);
}

// 重写基类Poller的抽象方法
TimeStamp EPollPoller::poll(int timeoutMs, ChannelList *activeChannels)
{
    // 实际使用LOG_DEBUG输出日志
    LOG_INFO("func = poll, fd total count: %lu \n", channels_.size());

    int numEvents = ::epoll_wait(epollfd_, events_.data(), static_cast<int>(events_.size()), timeoutMs);
    int saveErrno = errno;
    TimeStamp now(TimeStamp::now());
    
    if(numEvents > 0)
    {
        LOG_INFO("%d events happened \n", numEvents);
        fillActiveChannels(numEvents, activeChannels);
        if(numEvents == events_.size())
        {
            events_.resize(events_.size() * 2);
        }
    }
    else if(numEvents == 0)
    {
        LOG_DEBUG("poll timeout! \n");
    }
    else
    {
        if(saveErrno != EINTR)
        {
            errno = saveErrno; // 不会出现竟态条件?
            LOG_ERROR("EPollPoller::poll() error! \n");
        }
    }
    return now;
}

// channel update remove -> EventLoop updatechannel removechannel -> Poller updatechannel removechannel
void EPollPoller::updateChannel(Channel *channel)
{
    const int index = channel->index();
    LOG_INFO("func = updateChannel, fd = %d, events = %d, index = %d \n", channel->fd(), channel->events(), channel->index());
    if(index == kNew || index == kDeleted)
    {
        if(index == kNew)
        {
            int fd = channel->fd();
            channels_[fd] = channel;
        }
        channel->setIndex(kAdded);
        update(EPOLL_CTL_ADD, channel);
    }
    else // channel已经注册过了
    {
        int fd = channel->fd();
        if(channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL, channel);
            channel->setIndex(kDeleted);
        }
        else
        {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

// 填写活跃的连接,使得EventLoop获知
void EPollPoller::fillActiveChannels(int numEvents, ChannelList *activeChannels) const
{
    for(int i = 0; i < numEvents; ++i)
    {
        Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
        channel->set_revents(events_[i].events);
        activeChannels->push_back(channel); // EventLoop拿到了它的poller给它返回的所有发生事件的channel列表了
    }
}

// 更新Channel通道  epoll_ctl add/mod/del
void EPollPoller::update(int operation, Channel *channel)
{
    epoll_event event;
    memset(&event, 0, sizeof event);
    int fd = channel->fd();

    event.events = channel->events();
    event.data.fd = fd;
    event.data.ptr = channel;

    if(::epoll_ctl(epollfd_, operation, fd, &event) < 0)
    {
        if(operation == EPOLL_CTL_DEL)
        {
            LOG_ERROR("epoll_ctl del error: %d", errno);
        }
        else
        {
            LOG_FATAL("epoll_ctl del error: %d", errno);
        }
    }
}

// 从Poller中删除channel
void EPollPoller::removeChannel(Channel *channel)
{
    int fd = channel->fd();
    channels_.erase(fd);

    LOG_INFO("func = removeChannel, fd = %d", channel->fd());

    int index = channel->index();
    if(index == kAdded)
    {
        update(EPOLL_CTL_DEL, channel);
    }
    channel->setIndex(kNew);
}