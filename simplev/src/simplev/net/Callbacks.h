/*
 * Callbacks.h
 *
 *  Created on: Jun 13, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_CALLBACKS_H_
#define SIMPLEV_NET_CALLBACKS_H_

#include <boost/function.hpp>

namespace simplev
{
namespace net
{

// All client visible callbacks go here.

typedef boost::function<void()> TimerCallback;

}
}

#endif /* SIMPLEV_NET_CALLBACKS_H_ */
