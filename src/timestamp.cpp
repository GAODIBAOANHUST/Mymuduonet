#include <sys/time.h>
#include <inttypes.h>

#include "timestamp.h"

TimeStamp::TimeStamp():microSecondsSinceEpoch_(0)
{}

TimeStamp::TimeStamp(int64_t microSecondsSinceEpoch)
    :microSecondsSinceEpoch_(microSecondsSinceEpoch)
{}

TimeStamp TimeStamp::now()
{
    // 源码实现
    #if 0
    struct timeval tv;
    gettimeofday(&tv,nullptr);
    int64_t seconds = tv.tv_sec;
    return TimaStamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
    #endif

    return TimeStamp(time(NULL));
}

std::string TimeStamp::toString() const
{
    // 源码实现
    #if 0
    char buf[128] = {0};
    int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
    int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
    snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
    return buf;
    #endif

    char buf[128] = {0};
    tm* tm_time = localtime(&microSecondsSinceEpoch_);
    snprintf(buf, 128, "%4d/%02d%02d %02d:%02d:%02d", tm_time->tm_yday + 1900, tm_time->tm_mon+1, tm_time->tm_mday, tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec);
    return buf;
}

// int main()
// {
//     std::cout << TimeStamp::now().toString() << std::endl;
//     return 0;
// }