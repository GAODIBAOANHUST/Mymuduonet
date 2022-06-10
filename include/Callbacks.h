#pragma once

#include <memory>
#include <functional>

#include "timestamp.h"

class Buffer;
class TcpConnection;

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using ConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
using CloseCallback =  std::function<void(const TcpConnectionPtr&)>;
using WriteCompleteCallback =  std::function<void(const TcpConnectionPtr&)>;

using MessageCallback = std::function<void(const TcpConnectionPtr&, Buffer*, TimeStamp)>;
using HighWaterMarkCallback = std::function<void(const TcpConnectionPtr&, size_t)>;

class Callbacks
{

};