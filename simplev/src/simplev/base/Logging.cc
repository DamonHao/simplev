/*
 * Logging.cc
 *
 *  Created on: Jun 19, 2014
 *      Author: damonhao
 */
#include <simplev/base/Logging.h>

#include <string.h>

namespace simplev
{
namespace base
{

__thread char t_errnobuf[512];
const char* Logger::strerror_tl(int savedErrno)
{
	return strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
}

}
}

