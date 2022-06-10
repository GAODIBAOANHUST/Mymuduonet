#include "Poller.h"
#include "EPollPoller.h"

#include <stdlib.h>

Poller* Poller::newDefaultPoller(EventLoop* loop)
{
    if(::getenv("MUDUO_USE_POLL"))
    {
        // 生成Poll的实例
        // add code...
        return nullptr;
    }
    else
    {
        // 生成EPoll的实例
        // add code...
        return new EPollPoller(loop);
    }
}