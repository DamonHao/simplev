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
namespace socket
{

/// Creates a non-blocking socket file descriptor,
/// abort if any error.

int createNonblockingOrDie();
void bindOrDie(int sockfd, const struct sockaddr_in& addr);
}
}
}

#endif /* SIMPLEV_NET_SOCKETSOPS_H_ */
