/*
 * TimerId.h
 *
 *  Created on: Jun 13, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_TIMERID_H_
#define SIMPLEV_NET_TIMERID_H_

#include <simplev/base/copyable.h>
//#include <ev++.h>

namespace simplev
{
namespace net
{

class Timer;

class TimerId: public simplev::copyable
{
	TimerId(Timer *timer) :
			timer_(timer)
	{
	}

	// default copy-ctor, dtor and assignment are okay

	friend class TimerQueue;
private:
	Timer *timer_;

};

}
}

#endif /* SIMPLEV_NET_TIMERID_H_ */
