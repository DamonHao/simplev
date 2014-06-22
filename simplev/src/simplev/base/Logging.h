/*
 * Logging.h
 *
 *  Created on: Jun 11, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_BASE_LOGGING_H_
#define SIMPLEV_BASE_LOGGING_H_

#include <stdio.h> //perror()
#include <stdlib.h> //abort()

namespace
{
template <typename T> T* checkNotNULL( T* const ptr)
{
	assert(ptr != NULL);
	return ptr;
}

template<typename To, typename From>
inline To implicit_cast(From const &f) {
    return f;
}

}


namespace simplev
{
namespace base
{

class Logger
{
public:
	static void perror(const char* errorInfo)
	{
		::perror(errorInfo);
	}

	static void perrorAndAbort(const char* errorInfo)
	{
		::perror(errorInfo);
		::abort();
	}

	static void printAndAbort(const char* message)
	{
		::puts(message);
		::abort();
	}

	static const char* strerror_tl(int savedErrno);

	static void puts(const char* message)
	{
		::puts(message);//note not to ignore ::, otherwise cause infinite loop;
	}
};

}
}

#endif /* SIMPLEV_BASE_LOGGING_H_ */
