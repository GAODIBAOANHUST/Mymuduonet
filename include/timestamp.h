#pragma once

#include <iostream>
#include <string>

class TimeStamp
{
public:
    TimeStamp();
    explicit TimeStamp(int64_t );

    static constexpr int kMicroSecondsPerSecond = 1000 * 1000;

    static TimeStamp now();
    std::string toString() const;

private:
    int64_t microSecondsSinceEpoch_;
};