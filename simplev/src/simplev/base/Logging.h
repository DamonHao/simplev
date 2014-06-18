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

};

}
}

#endif /* SIMPLEV_BASE_LOGGING_H_ */
