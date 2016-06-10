#include <iostream>
#include <algorithm>
#include <set>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>
#include <string.h>

int set_nonblock(int fd)
{
	int flags;
#if defined(O_NONBLOCK)
	if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
		flags = 0;
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
	flags = 1;
	return ioctl(fd, FIOBIO, &flags);
#endif
} 

int main(int argc, char **argv)
{
	int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	std::set<int> SlaveSockets;

	if(MasterSocket == -1)
	{
		std::cout << strerror(errno) << std::endl;
		return 1;
	}

	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(12345);
	SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int Result = bind(MasterSocket, (struct sockaddr *)&SockAddr, sizeof(SockAddr));

	if(Result == -1)
	{
		std::cout << strerror(errno) << std::endl;
		return 1;
	}

	set_nonblock(MasterSocket);

	Result = listen(MasterSocket, SOMAXCONN);

	if(Result == -1)
	{
		std::cout << strerror(errno) << std::endl;
		return 1;
	}

	while(true)
	{
		fd_set Set;
		FD_ZERO(&Set);
		FD_SET(MasterSocket, &Set);
		for(auto Iter = SlaveSockets.begin(); Iter != SlaveSockets.end(); Iter++) FD_SET(*Iter, &Set);

		int Max = std::max(MasterSocket, *std::max_element(SlaveSockets.begin(), SlaveSockets.end()));

		if(select(Max+1, &Set, NULL, NULL, NULL) <= 0)
		{
			std::cout << strerror(errno) << std::endl;
			return 1;
		}

		for(auto Iter = SlaveSockets.begin(); Iter != SlaveSockets.end(); Iter++)
		{
			if(FD_ISSET(*Iter, &Set))
			{
				static char Buffer[1024];
				int RecvSize = recv(*Iter, Buffer, 1024, MSG_NOSIGNAL);
				if((RecvSize == 0) && (errno != EAGAIN))
				{
					shutdown(*Iter, SHUT_RDWR);
					close(*Iter);
					SlaveSockets.erase(Iter);
				}
				else if(RecvSize != 0)
				{
					send(*Iter, Buffer, RecvSize, MSG_NOSIGNAL);
				}
			}
		}

		if(FD_ISSET(MasterSocket, &Set))
		{
			int SlaveSocket = accept(MasterSocket, 0, 0);
			set_nonblock(SlaveSocket);
			SlaveSockets.insert(SlaveSocket);
		}

	}

	return 0;
}
