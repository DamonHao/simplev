/*
 * Exception.cc
 *
 *  Created on: Jun 26, 2014
 *      Author: damonhao
 */
#include <simplev/base/Exception.h>

//#include <cxxabi.h>
#include <execinfo.h>
#include <stdlib.h>

using namespace simplev;

Exception::Exception(const char* msg)
  : message_(msg)
{
  fillStackTrace();
}

Exception::Exception(const std::string& msg)
  : message_(msg)
{
  fillStackTrace();
}

Exception::~Exception() throw ()
{
}

const char* Exception::what() const throw()
{
  return message_.c_str();
}

const char* Exception::stackTrace() const throw()
{
  return stack_.c_str();
}

void Exception::fillStackTrace()
{
  const int len = 200;
  void* buffer[len];
  int nptrs = ::backtrace(buffer, len);
  char** strings = ::backtrace_symbols(buffer, nptrs);
  if (strings)
  {
    for (int i = 0; i < nptrs; ++i)
    {
      // TODO demangle funcion name with abi::__cxa_demangle
      stack_.append(strings[i]);
      stack_.push_back('\n');
    }
    free(strings);
  }
}



