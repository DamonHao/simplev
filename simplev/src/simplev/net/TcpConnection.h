/*
 * TcpConnection.h
 *
 *  Created on: Jun 18, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_TCPCONNECTION_H_
#define SIMPLEV_NET_TCPCONNECTION_H_

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <simplev/net/Buffer.h>
#include <simplev/net/Callbacks.h>
#include <simplev/net/InetAddress.h>

namespace simplev
{
namespace net
{

class Channel;
class EventLoop;
class Socket;

/// TCP connection, for both client and server usage.
class TcpConnection: boost::noncopyable,
										public boost::enable_shared_from_this<TcpConnection>
{
public:
	/// Constructs a TcpConnection with a connected sockfd
  /// User should not create this object.
	TcpConnection(EventLoop *loop,
								const std::string& name,
								int sockfd,
								const InetAddress& localAddr,
								const InetAddress& peerAddr);
	 ~TcpConnection();
	 EventLoop* getLoop() const { return loop_; }
	 const std::string& name() const { return name_; }
	 const InetAddress& localAddress() { return localAddr_; }
	 const InetAddress& peerAddress() { return peerAddr_; }
	 bool connected() const { return state_ == kConnected; }

	 // Thread safe.
	 void send(const std::string& message);
	 // Thread safe.
	 void shutdown();

	 void setKeepAlive(bool on);
	 void setTcpNoDelay(bool on);
	 void setConnectionCallback(const ConnectionCallback& cb)
	 { connectionCallback_ = cb; }

	 void setMessageCallback(const MessageCallback& cb)
	 { messageCallback_ = cb; }

		void setCloseCallback(const CloseCallback& cb)
		{ closeCallback_ = cb; }

	 /// Internal use only.
	// called when TcpServer accepts a new connection
	void connectEstablished();   // should be called only once
  // called when TcpServer has removed me from its map
  void connectDestroyed();  // should be called only once




private:
	enum StateE { kConnecting, kConnected, kDisconnecting, kDisconnected};

	void setState(StateE s) { state_ = s; }
//	void handleRead();
	void handleRead(Timestamp receiveTime );
	void handleWrite();
	void handleClose();
	void handleError();
  void sendInLoop(const std::string& message);
  void shutdownInLoop();

	EventLoop* loop_;
	std::string name_;
	StateE state_;  // FIXME: use atomic variable
	// we don't expose those classes to client.
	boost::scoped_ptr<Socket> socket_;
	boost::scoped_ptr<Channel> channel_;
	InetAddress localAddr_;
	InetAddress peerAddr_;
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
	CloseCallback closeCallback_;
	Buffer inputBuffer_;
	Buffer outputBuffer_;
};

}
}

#endif /* SIMPLEV_NET_TCPCONNECTION_H_ */
