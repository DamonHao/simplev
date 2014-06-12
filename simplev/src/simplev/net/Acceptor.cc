/*
 * Acceptor.cc
 *
 *  Created on: Jun 11, 2014
 *      Author: damonhao
 */

#include <simplev/net/Acceptor.h>
#include <simplev/net/SocketsOps.h>

using namespace ev;
using namespace simplev::net;

Acceptor::Acceptor(loop_ref loop, const InetAddress& listenAddr) :
		loop_(loop), acceptSocket_(socket::createNonblockingOrDie())
{

}

