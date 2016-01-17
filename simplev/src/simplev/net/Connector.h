/*
 * Connector.h
 *
 *  Created on: Jun 23, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_CONNECTOR_H_
#define SIMPLEV_NET_CONNECTOR_H_

#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <simplev/net/InetAddress.h>
#include <simplev/net/TimerId.h>

namespace simplev
{
namespace net
{

class Channel;
class EventLoop;

class Connector : boost::noncopyable,
									public boost::enable_shared_from_this<Connector>
{
public:
	typedef boost::function<void(int sockfd)> NewConnectionCallback;
	Connector(EventLoop* loop, const InetAddress& serverAddr);
	~Connector();

  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { newConnectionCallback_ = cb; }

  void start();  // can be called in any thread
  void restart();  // must be called in loop thread
  void stop();  // can be called in any thread

  const InetAddress& serverAddress() const { return serverAddr_; }

private:
	enum States { kDisconnected, kConnecting, kConnected };

  static const int kMaxRetryDelayMs = 30*1000;
  static const int kInitRetryDelayMs = 500;

  void setState(States s) { state_ = s; }
  void startInLoop();
  void stopInLoop();
  void connect();
  void connecting(int sockfd);
  void handleWrite();
  void handleError();
  void retry(int sockfd);
  int removeAndResetChannel();
  void resetChannel();

	EventLoop* loop_;
	InetAddress serverAddr_;
	bool connect_; // atomic
	States state_;  // FIXME: use atomic variable
	boost::scoped_ptr<Channel> channel_;
	NewConnectionCallback newConnectionCallback_;
	int retryDelayMs_;
	TimerId timerId_;

};
typedef boost::shared_ptr<Connector> ConnectorPtr;
}
}

#endif /* SIMPLEV_NET_CONNECTOR_H_ */
