/*
 * Curl.h
 *
 *  Created on: Jul 2, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_CASES_CURL_CURL_H_
#define SIMPLEV_CASES_CURL_CURL_H_

#include <simplev/base/StringPiece.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/scoped_ptr.hpp>

extern "C"
{
typedef void CURLM;
typedef void CURL;
}

namespace simplev
{
namespace net
{
class Channel;
class EventLoop;
}
}

namespace curl
{

class Curl;

class Request : public boost::enable_shared_from_this<Request>,
                boost::noncopyable
{
 public:
  typedef boost::function<void(const char*, int)> DataCallback;
  typedef boost::function<void(Request*, int)> DoneCallback;

  Request(Curl*, const char* url);
  ~Request();

  void setDataCallback(const DataCallback& cb)
  { dataCb_ = cb; }

  void setDoneCallback(const DoneCallback& cb)
  { doneCb_ = cb; }

  void setHeaderCallback(const DataCallback& cb)
  { headerCb_ = cb; }

  // void allowRedirect(int redirects);
  void headerOnly();
  void setRange(const simplev::StringArg range);

  template<typename OPT>
  int setopt(OPT opt, long p)
  {
    return curl_easy_setopt(curl_, opt, p);
  }

  template<typename OPT>
  int setopt(OPT opt, const char* p)
  {
    return curl_easy_setopt(curl_, opt, p);
  }

  template<typename OPT>
  int setopt(OPT opt, void* p)
  {
    return curl_easy_setopt(curl_, opt, p);
  }

  template<typename OPT>
  int setopt(OPT opt, size_t (*p)(char *, size_t , size_t , void *))
  {
    return curl_easy_setopt(curl_, opt, p);
  }

  const char* getEffectiveUrl();
  const char* getRedirectUrl();
  int getResponseCode();

  // internal
  simplev::net::Channel* setChannel(int fd);
  void removeChannel();
  void done(int code);
  CURL* getCurl() { return curl_; }
  simplev::net::Channel* getChannel() { return get_pointer(channel_); }

 private:

  void dataCallback(const char* buffer, int len);
  void headerCallback(const char* buffer, int len);
  static size_t writeData(char *buffer, size_t size, size_t nmemb, void *userp);
  static size_t headerData(char *buffer, size_t size, size_t nmemb, void *userp);
  void doneCallback();

  class Curl* owner_;
  CURL* curl_;
  boost::shared_ptr<simplev::net::Channel> channel_;
  DataCallback dataCb_;
  DataCallback headerCb_;
  DoneCallback doneCb_;
};

typedef boost::shared_ptr<Request> RequestPtr;

class Curl : boost::noncopyable
{
 public:

  enum Option
  {
    kCURLnossl = 0,
    kCURLssl   = 1,
  };

  explicit Curl(simplev::net::EventLoop* loop);
  ~Curl();

  RequestPtr getUrl(simplev::StringArg url);

  static void initialize(Option opt = kCURLnossl);

  // internal
  CURLM* getCurlm() { return curlm_; }
  simplev::net::EventLoop* getLoop() { return loop_; }

 private:
  void onTimer();
  void onRead(int fd);
  void onWrite(int fd);
  void checkFinish();

  static int socketCallback(CURL*, int, int, void*, void*);
  static int timerCallback(CURLM*, long, void*);

  simplev::net::EventLoop* loop_;
  CURLM* curlm_;
  int runningHandles_;
  int prevRunningHandles_;
};

}



#endif /* SIMPLEV_CASES_CURL_CURL_H_ */
