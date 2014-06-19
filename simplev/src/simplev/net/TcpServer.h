/*
 * TcpServer.h
 *
 *  Created on: Jun 11, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_TCPSERVER_H_
#define SIMPLEV_NET_TCPSERVER_H_

#include <map>

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <simplev/net/Callbacks.h>

namespace simplev
{
namespace net
{

class Acceptor;
class EventLoop;
class InetAddress;

class TcpServer: boost::noncopyable
{
public:
	TcpServer(EventLoop *loop, const InetAddress& listenAddr);
	~TcpServer();  // force out-line dtor, for scoped_ptr members.

  /// Starts the server if it's not listenning.
  /// It's harmless to call it multiple times.
  /// Thread safe.
  void start();
  /// Set connection callback.
  /// Not thread safe.
  void setConnectionCallback(const ConnectionCallback& cb)
  { connectionCallback_ = cb; }

  /// Set message callback.
  /// Not thread safe.
  void setMessageCallback(const MessageCallback& cb)
  { messageCallback_ = cb; }
private:
	/// Not thread safe, but in loop
	void newConnection(int sockfd, const InetAddress& peerAddr);
	void removeConnection(const TcpConnectionPtr& conn);

	typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

	EventLoop* loop_;  // the acceptor loop
	const std::string name_;
	boost::scoped_ptr<Acceptor> acceptor_; // avoid revealing Acceptor
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
	bool started_;
	int nextConnId_;  // always in loop thread
	ConnectionMap connections_;

};

}
}

#endif /* SIMPLEV_NET_TCPSERVER_H_ */
