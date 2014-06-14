/*
 * Channel.h
 *
 *  Created on: Jun 12, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_CHANNEL_H_
#define SIMPLEV_NET_CHANNEL_H_

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <ev++.h>

namespace simplev
{
namespace net
{

class EventLoop;
/// A selectable I/O channel.
///
/// This class doesn't own the file descriptor.
/// The file descriptor could be a socket,
/// an eventfd, a timerfd, or a signalfd
class Channel: boost::noncopyable
{
public:
	typedef boost::function<void()> EventCallback;
	Channel(EventLoop* loop, int fd);
//	~Channel();
	void enableReading(){ioWatcher_.set(fd_, ioWatcher_.events | ev::READ);}
	void disableReading(){ioWatcher_.set(fd_, ioWatcher_.events & ~ev::READ);}
	void enableWriting(){ioWatcher_.set(fd_, ioWatcher_.events | ev::WRITE);}
	void disableWriting(){ioWatcher_.set(fd_, ioWatcher_.events & ~ev::WRITE);}
	void disableAll(){ioWatcher_.set(fd_, ioWatcher_.events & ev::NONE);}

	void setReadCallback(const EventCallback& cb)
	{
		readCallback_ = cb;
	}
	void setWriteCallback(const EventCallback& cb)
	{
		writeCallback_ = cb;
	}
	void setErrorCallback(const EventCallback& cb)
	{
		errorCallback_ = cb;
	}
private:
	void handleEvent(ev::io &io_watcher, int revents);

	EventLoop* loop_;
	const int fd_;
	ev::io ioWatcher_;

	EventCallback readCallback_;
	EventCallback writeCallback_;
	EventCallback errorCallback_;
};

}
}

#endif /* SIMPLEV_NET_CHANNEL_H_ */
