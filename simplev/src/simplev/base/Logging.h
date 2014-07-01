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

#include <simplev/base/LogStream.h>
#include <simplev/base/Timestamp.h>

namespace
{
//template <typename T> T* checkNotNULL( T* const ptr)
//{
//	assert(ptr != NULL);
//	return ptr;
//}

template<typename To, typename From>
inline To implicit_cast(From const &f) {
    return f;
}

}


namespace simplev
{
namespace base
{

//class Logger
//{
//public:
//	static void perror(const char* errorInfo)
//	{
//		::perror(errorInfo);
//	}
//
//	static void perrorAndAbort(const char* errorInfo)
//	{
//		::perror(errorInfo);
//		::abort();
//	}
//
//	static void printAndAbort(const char* message)
//	{
//		::puts(message);
//		::abort();
//	}
//
//	static const char* strerror_tl(int savedErrno);
//
//	static void puts(const char* message)
//	{
//		::puts(message);//note not to ignore ::, otherwise cause infinite loop;
//	}
//};

}

class Logger
{
 public:
  enum LogLevel
  {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
  };

  Logger(const char* file, int line);
  Logger(const char* file, int line, LogLevel level);
  Logger(const char* file, int line, LogLevel level, const char* func);
  Logger(const char* file, int line, bool toAbort);
  ~Logger();

  LogStream& stream() { return impl_.stream_; }

  static LogLevel logLevel();
  static void setLogLevel(LogLevel level);

  typedef void (*OutputFunc)(const char* msg, int len);
  typedef void (*FlushFunc)();
  static void setOutput(OutputFunc);
  static void setFlush(FlushFunc);

 private:

class Impl
{
 public:
  typedef Logger::LogLevel LogLevel;
  Impl(LogLevel level, int old_errno, const char* file, int line);
  void formatTime();
  void finish();

  Timestamp time_;
  LogStream stream_;
  LogLevel level_;
  int line_;
  const char* fullname_;
  const char* basename_;
};

  Impl impl_;

};

#define LOG_TRACE if (simplev::Logger::logLevel() <= simplev::Logger::TRACE) \
  simplev::Logger(__FILE__, __LINE__, simplev::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (simplev::Logger::logLevel() <= simplev::Logger::DEBUG) \
  simplev::Logger(__FILE__, __LINE__, simplev::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (simplev::Logger::logLevel() <= simplev::Logger::INFO) \
  simplev::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN simplev::Logger(__FILE__, __LINE__, simplev::Logger::WARN).stream()
#define LOG_ERROR simplev::Logger(__FILE__, __LINE__, simplev::Logger::ERROR).stream()
#define LOG_FATAL simplev::Logger(__FILE__, __LINE__, simplev::Logger::FATAL).stream()
#define LOG_SYSERR simplev::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL simplev::Logger(__FILE__, __LINE__, true).stream()

const char* strerror_tl(int savedErrno);

// Taken from glog/logging.h
//
// Check that the input is non NULL.  This very useful in constructor
// initializer lists.

#define CHECK_NOTNULL(val) \
  ::simplev::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(const char *file, int line, const char *names, T* ptr) {
  if (ptr == NULL) {
   Logger(file, line, Logger::FATAL).stream() << names;
  }
  return ptr;
}

}

#endif /* SIMPLEV_BASE_LOGGING_H_ */
