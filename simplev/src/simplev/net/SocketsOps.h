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

inline uint64_t hostToNetwork64(uint64_t host64)
{
	return htobe64(host64);
}

inline uint32_t hostToNetwork32(uint32_t host32)
{
	return htonl(host32);
}

inline uint16_t hostToNetwork16(uint16_t host16)
{
	return htons(host16);
}

inline uint64_t networkToHost64(uint64_t net64)
{
  return be64toh(net64);
}

inline uint32_t networkToHost32(uint32_t net32)
{
  return be32toh(net32);
}

inline uint16_t networkToHost16(uint16_t net16)
{
  return be16toh(net16);
}

void toIpPort(char* buf, size_t size, const struct sockaddr_in& addr);

/// Creates a non-blocking socket file descriptor,
/// abort if any error.
int createNonblockingOrDie();

void bindOrDie(int sockfd, const struct sockaddr_in& addr);
void listenOrDie(int sockfd);
int accept(int sockfd, struct sockaddr_in* addr);

ssize_t read(int sockfd, void *buf, size_t count);
ssize_t write(int sockfd, const void *buf, size_t count);
void close(int fd);

void fromHostPort(const char* ip, uint16_t port, struct sockaddr_in* addr);

}
}
}

#endif /* SIMPLEV_NET_SOCKETSOPS_H_ */
