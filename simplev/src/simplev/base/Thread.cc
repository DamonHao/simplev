/*
 * Thread.cc
 *
 *  Created on: Jun 12, 2014
 *      Author: damonhao
 */

#include <simplev/base/CurrentThread.h>

#include <assert.h>
#include <unistd.h>
#include <stdio.h> // snprintf()
#include <sys/syscall.h>

namespace simplev
{
namespace CurrentThread
{
__thread int t_cachedTid = 0;
__thread char t_tidString[32];

}

namespace detail
{

pid_t gettid()
{
	return static_cast<pid_t>(::syscall(SYS_gettid));
}

}

}

using namespace simplev;

void CurrentThread::cacheTid()
{
	if (t_cachedTid == 0)
	{
		t_cachedTid = detail::gettid();
		int n = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
		assert(n == 6);
//		(void) n;
	}
}
