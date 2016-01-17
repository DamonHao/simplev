/*
 * Acceptor.h
 *
 *  Created on: Jun 11, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_ACCEPTOR_H_
#define SIMPLEV_NET_ACCEPTOR_H_

#include <simplev/net/Socket.h>

//#include <ev++.h>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include <simplev/net/Channel.h>

namespace simplev
{
namespace net
{

class EventLoop;

class Acceptor: boost::noncopyable
{
public:
	 typedef boost::function<void (int sockfd,
	                                const InetAddress&)> NewConnectionCallback;

	Acceptor(EventLoop *loop, const InetAddress& listenAddr);
	~Acceptor();
	void setNewConnectionCallback(const NewConnectionCallback& cb)
	{ newConnectionCallback_ = cb;}
	bool listening() const {return listening_;}
	void listen();

private:
	void handleRead();

	EventLoop * loop_;
	Socket acceptSocket_;
	Channel acceptChannel_;
	NewConnectionCallback newConnectionCallback_;
	bool listening_;
	int idleFd_;
//	ev::loop_ref loop_;
};

}
}

#endif /* SIMPLEV_NET_ACCEPTOR_H_ */
