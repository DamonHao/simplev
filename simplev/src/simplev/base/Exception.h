/*
 * Exception.h
 *
 *  Created on: Jun 26, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_BASE_EXCEPTION_H_
#define SIMPLEV_BASE_EXCEPTION_H_

#include <exception>
#include <string>

namespace simplev
{

class Exception : public std::exception
{
 public:
  explicit Exception(const char* what);
  explicit Exception(const std::string& what);
  virtual ~Exception() throw();
  virtual const char* what() const throw();
  const char* stackTrace() const throw();

 private:
  void fillStackTrace();

  std::string message_;
  std::string stack_;
};

}





#endif /* SIMPLEV_BASE_EXCEPTION_H_ */
