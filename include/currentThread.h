#pragma once

#include <unistd.h>
#include <syscall.h>

namespace CurrentThread
{
    extern __thread int t_cachedTid;

    void cachedTid();

    inline int tid()
    {
        if(__builtin_expect(t_cachedTid == 0, 0))
        {
            // 还没有获取过
            cachedTid();
        }
        return t_cachedTid;
    }
}