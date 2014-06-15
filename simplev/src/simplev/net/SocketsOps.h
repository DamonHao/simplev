/*
 * SocketsOps.h
 *
 *  Created on: Jun 11, 2014
 *      Author: damonhao
 */

#ifndef SIMPLEV_NET_SOCKETSOPS_H_
#define SIMPLEV_NET_SOCKETSOPS_H_

#include <arpa/inet.h>

namespace simplev
{
namespace net
{
namespace sockets
{

/// Creates a non-blocking socket file descriptor,
/// abort if any error.

int createNonblockingOrDie();
void bindOrDie(int sockfd, const struct sockaddr_in& addr);
ssize_t read(int sockfd, void *buf, size_t count);
ssize_t write(int sockfd, const void *buf, size_t count);
}
}
}

#endif /* SIMPLEV_NET_SOCKETSOPS_H_ */
