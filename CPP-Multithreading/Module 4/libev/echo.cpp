#include <iostream>
#include <algorithm>
#include <set>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include <ev.h>

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

void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	if(EV_ERROR & revents)
	{
		return;
	}

	static char Buffer[1024];

	int RecvSize = recv(watcher->fd, Buffer, 1024, MSG_NOSIGNAL);
	if(RecvSize <= 0)
	{
		ev_io_stop(loop, watcher);
		free(watcher);
	}
	send(watcher->fd, Buffer, RecvSize, MSG_NOSIGNAL);
}

void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
	struct ev_io *w_client = (struct ev_io*) malloc (sizeof(struct ev_io));

	if(EV_ERROR & revents)
	{
		return;
	}

	int SlaveSocket = accept(watcher->fd, 0, 0);

	if(SlaveSocket < 0)
	{
		return;
	}

	set_nonblock(SlaveSocket);

	ev_io_init(w_client, read_cb, SlaveSocket, EV_READ);
	ev_io_start(loop, w_client);
}

int main(int argc, char **argv)
{
	struct ev_loop *loop = ev_default_loop(0);
	struct ev_io w_accept;

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

	ev_io_init(&w_accept, accept_cb, MasterSocket, EV_READ);
	ev_io_start(loop, &w_accept);

	ev_run(loop);

	return 0;
}
