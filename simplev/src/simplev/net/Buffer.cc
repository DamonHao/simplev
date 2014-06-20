/*
 * Buffer.cc
 *
 *  Created on: Jun 19, 2014
 *      Author: damonhao
 */
#include <simplev/net/Buffer.h>

#include <errno.h>
#include <sys/uio.h> //readv()

#include <simplev/base/Logging.h>
#include <simplev/net/SocketsOps.h>

using namespace simplev::net;

ssize_t Buffer::readFd(int fd, int* savedErrno)
{
	//use stack space, we reduce the times of system call;
  char extrabuf[65536];
  struct iovec vec[2];
  const size_t writable = writableBytes();
  vec[0].iov_base = begin()+writerIndex_;
  vec[0].iov_len = writable;
  vec[1].iov_base = extrabuf;
  vec[1].iov_len = sizeof extrabuf;
//  const ssize_t n = readv(fd, vec, 2);
  const ssize_t n = sockets::readv(fd, vec, 2);
  if (n < 0) {
    *savedErrno = errno;
  } else if (implicit_cast<size_t>(n) <= writable) {
    writerIndex_ += n;
  } else {
    writerIndex_ = buffer_.size();
    append(extrabuf, n - writable);
  }
  return n;
}



