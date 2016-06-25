#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	key_t key = ftok( "/tmp/mem.temp", 1 );
	int shmid = shmget( key, 1024*1024, IPC_CREAT | 0666 );
	char* attached = (char*)shmat( shmid, 0, 0 );

	for( int i = 0; i < 1024*1024; i++ ) {
		attached[i] = 42;
	}
	shmdt( attached );
	return 0;
}
