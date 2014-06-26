/*
 * CurrentThread.h
 *
 *  Created on: Jun 12, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_BASE_CURRENTTHREAD_H_
#define SIMPLEV_BASE_CURRENTTHREAD_H_

namespace simplev
{
namespace CurrentThread
{
// internal
extern __thread int t_cachedTid;
extern __thread char t_tidString[32];

void cacheTid();
inline int tid()
{
	if (__builtin_expect(t_cachedTid == 0, 0))
	{
		cacheTid();
	}
	return t_cachedTid;
}

inline const char* tidString() // for logging
{
  return t_tidString;
}

bool isMainThread();

}
}

#endif /* SIMPLEV_BASE_CURRENTTHREAD_H_ */
