/*
 *  Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant 
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "EmptyHeader.h"

#ifdef RAKNET_SOCKET_2_INLINE_FUNCTIONS

#ifndef RAKNETSOCKET2_WINDOWS_LINUX_CPP
#define RAKNETSOCKET2_WINDOWS_LINUX_CPP

#if !defined(WINDOWS_STORE_RT) && !defined(__native_client__)

#if (defined(__GNUC__)  || defined(__GCCXML__)) && !defined(__WIN32__)
#include <netdb.h>
#endif

#if RAKNET_SUPPORT_IPV6==1

void PrepareAddrInfoHints2(addrinfo *hints)
{
	memset(hints, 0, sizeof (addrinfo)); // make sure the struct is empty
	hints->ai_socktype = SOCK_DGRAM; // UDP sockets
	hints->ai_flags = AI_PASSIVE;     // fill in my IP for me
}

void GetMyIP_Windows_Linux_IPV4And6( SystemAddress addresses[MAXIMUM_NUMBER_OF_INTERNAL_IDS] )
{
	int idx, written;
	ifaddrs *ifap, *it;
	int err = getifaddrs(&ifap);
	RakAssert(err != -1);
	
	for (idx=0, written=0, it=ifap; it && idx < MAXIMUM_NUMBER_OF_INTERNAL_IDS; it = it->ifa_next, idx++)
	{
		if (!it->ifa_addr) continue;
		
		if (it->ifa_addr->sa_family == AF_INET)
		{
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)it->ifa_addr;
			memcpy(&addresses[written++].address.addr4,ipv4,sizeof(sockaddr_in));
		}
		else if (it->ifa_addr->sa_family == AF_INET6)
		{
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)it->ifa_addr;
			memcpy(&addresses[written++].address.addr4,ipv6,sizeof(sockaddr_in6));
		}

	}

	freeifaddrs(ifap);
	
	while (written < MAXIMUM_NUMBER_OF_INTERNAL_IDS)
	{
		addresses[written]=UNASSIGNED_SYSTEM_ADDRESS;
		written++;
	}
}

#else

void GetMyIP_Windows_Linux_IPV4( SystemAddress addresses[MAXIMUM_NUMBER_OF_INTERNAL_IDS] )
{



	int idx=0;
	char ac[ 80 ];
	int err = gethostname( ac, sizeof( ac ) );
    (void) err;
	RakAssert(err != -1);
	
	struct hostent *phe = gethostbyname( ac );

	if ( phe == 0 )
	{
		RakAssert(phe!=0);
		return ;
	}
	for ( idx = 0; idx < MAXIMUM_NUMBER_OF_INTERNAL_IDS; ++idx )
	{
		if (phe->h_addr_list[ idx ] == 0)
			break;

		memcpy(&addresses[idx].address.addr4.sin_addr,phe->h_addr_list[ idx ],sizeof(struct in_addr));
	}
	
	while (idx < MAXIMUM_NUMBER_OF_INTERNAL_IDS)
	{
		addresses[idx]=UNASSIGNED_SYSTEM_ADDRESS;
		idx++;
	}

}

#endif // RAKNET_SUPPORT_IPV6==1


void GetMyIP_Windows_Linux( SystemAddress addresses[MAXIMUM_NUMBER_OF_INTERNAL_IDS] )
{
	#if RAKNET_SUPPORT_IPV6==1
		GetMyIP_Windows_Linux_IPV4And6(addresses);
	#else
		GetMyIP_Windows_Linux_IPV4(addresses);
	#endif
}


#endif // Windows and Linux

#endif // file header

#endif // #ifdef RAKNET_SOCKET_2_INLINE_FUNCTIONS
