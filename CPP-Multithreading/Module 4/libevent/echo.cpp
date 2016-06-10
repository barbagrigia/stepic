#include <iostream>
#include <algorithm>
#include <set>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include <event.h>

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

struct client
{
        struct event ev_read;
};

void on_read(int fd, short ev, void *arg)
{
	struct client *client = (struct client *)arg;
	
	static char Buffer[1024];
	int RecvSize = recv(fd, Buffer, 1024, MSG_NOSIGNAL);
	if(RecvSize <= 0)
	{
		close(fd);
		event_del(&client->ev_read);
		free(client);
	}
	send(fd, Buffer, RecvSize, MSG_NOSIGNAL);
}

void on_accept(int fd, short ev, void *arg)
{
	int SlaveSocket = accept(fd, 0, 0);
	set_nonblock(SlaveSocket);

	struct client *client;
	client = (struct client *) calloc(1, sizeof(*client));
	event_set(&client->ev_read, SlaveSocket, EV_READ | EV_PERSIST, on_read, client);
	event_add(&client->ev_read, NULL);
}

int main(int argc, char **argv)
{
	event_init();

	int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

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

	struct event ev_accept;
	event_set(&ev_accept, MasterSocket, EV_READ | EV_PERSIST, on_accept, NULL);
	event_add(&ev_accept, NULL);
	event_dispatch();

	return 0;
}
