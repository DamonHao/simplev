/*
 * TimerId.h
 *
 *  Created on: Jun 13, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_TIMERID_H_
#define SIMPLEV_NET_TIMERID_H_

#include <simplev/base/copyable.h>

namespace simplev
{
namespace net
{

class Timer;

class TimerId: public simplev::copyable
{
public:
	TimerId(Timer* timer = NULL, int64_t seq = 0) :
			timer_(timer),
			sequence_(seq)
	{
	}

	// default copy-ctor, dtor and assignment are okay

	friend class TimerQueue;
private:
	Timer *timer_;
	int64_t sequence_;
};

}
}

#endif /* SIMPLEV_NET_TIMERID_H_ */
