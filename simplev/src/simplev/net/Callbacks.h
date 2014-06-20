/*
 * Callbacks.h
 *
 *  Created on: Jun 13, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_CALLBACKS_H_
#define SIMPLEV_NET_CALLBACKS_H_

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <simplev/base/Timestamp.h>

namespace simplev
{
namespace net
{

// All client visible callbacks go here.

typedef boost::function<void()> TimerCallback;
class Buffer;
class TcpConnection;
typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef boost::function<void (const TcpConnectionPtr&)> ConnectionCallback;
//typedef boost::function<void (const TcpConnectionPtr&,
//                              const char* data,
//                              ssize_t len)> MessageCallback;
typedef boost::function<void (const TcpConnectionPtr&,
                              Buffer* buf,
                              Timestamp)> MessageCallback;

typedef boost::function<void (const TcpConnectionPtr&)> CloseCallback;

}
}

#endif /* SIMPLEV_NET_CALLBACKS_H_ */
