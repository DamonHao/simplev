/*
 * TcpClient.h
 *
 *  Created on: Jun 24, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_TCPCLIENT_H_
#define SIMPLEV_NET_TCPCLIENT_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <muduo/base/Mutex.h>

#include <simplev/net/TcpConnection.h>

namespace simplev
{
namespace net
{

class Connector;
typedef boost::shared_ptr<Connector> ConnectorPtr;

class TcpClient: boost::noncopyable
{
public:
	TcpClient(EventLoop* loop, const InetAddress& serverAddr);
	~TcpClient();  // force out-line dtor, for scoped_ptr members.

	void connect(); //thread safe
	void disconnect(); //thread safe
	void stop(); //thread safe

	bool retry() const;
	void enableRetry() { retry_ = true; }

  // Set connection callback.
  // Not thread safe.
  void setConnectionCallback(const ConnectionCallback& cb)
  { connectionCallback_ = cb; }
  // Set message callback.
  // Not thread safe.
  void setMessageCallback(const MessageCallback& cb)
  { messageCallback_ = cb; }
  // Set write complete callback.
  // Not thread safe.
  void setWriteCompleteCallback(const WriteCompleteCallback& cb)
  { writeCompleteCallback_ = cb; }

private:
  /// Not thread safe, but in loop
  void newConnection(int sockfd);
  /// Not thread safe, but in loop
  void removeConnection(const TcpConnectionPtr& conn);

	EventLoop* loop_;
	ConnectorPtr connector_; // avoid revealing Connector
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
	WriteCompleteCallback writeCompleteCallback_;
	bool retry_;   // atmoic
	bool connect_; // atomic
	// always in loop thread
	int nextConnId_;
	mutable muduo::MutexLock mutex_;
	TcpConnectionPtr connection_; // @BuardedBy mutex_
};

}
}

#endif /* SIMPLEV_NET_TCPCLIENT_H_ */
