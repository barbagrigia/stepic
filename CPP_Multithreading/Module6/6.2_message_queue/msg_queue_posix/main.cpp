#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>

const char* mq_name = "/test.mq";

int main()
{
	if( 0 == mq_unlink( mq_name ) ) {
		printf( "Message queue was removed\n" );
	}
	
	mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 128;
	attr.mq_curmsgs = 0;
	
	mqd_t mq = mq_open( mq_name, O_CREAT | O_RDWR, 0666, &attr );

	char buffer[256];
	memset(  buffer, 0, 256 );
	size_t size = mq_receive( mq, buffer, 256, 0 );


	int fd = open( "/home/box/message.txt", O_CREAT | O_WRONLY | O_TRUNC, 0666  );
	if( fd < 0 ) {
		printf( "Cant open out file" );
	}
	write( fd, buffer, size );
	close( fd );
	mq_unlink( mq_name );
	return 0;
}
