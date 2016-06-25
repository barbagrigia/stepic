#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

const char* sysV_key_path = "/tmp/msg.temp";
const char* out_message_path = "/home/box/message.txt";

struct message {
    long mtype;
    char mtext[80];
};

int main()
{
	int key_res = creat( sysV_key_path, 0666  );
	if( key_res == -1 ) {
		printf( "Error. creat temp\n" );
		return -1;
	}

	key_t key = ftok( sysV_key_path	, 1 );

	int msgqid = msgget( key, IPC_CREAT | 0666 );
	if( msgqid == -1 ) {
		printf( "Error. cant get message queue: %d\n", errno );
		return -1;
	}

	struct message msg;

	size_t len = msgrcv( msgqid, &msg, 80, 0, 0 );
	if( len == -1 ) {
		printf( "Error. cant receive message: %d\n", errno );
			
		return -1;
	}
	
	int out_file = open( out_message_path, O_CREAT | O_WRONLY | O_TRUNC );
	if( out_file == -1 ) {
		printf( "Error. open out file\n" );
		return -1;
	}

	size_t writen = write( out_file, msg.mtext, len );
	if( writen < len ) {
		printf( "Error. write out file\n" );
		return -1;
	}

	msgctl( msgqid, IPC_RMID, 0 );
	unlink( sysV_key_path );	
	
	return 0;
}
