#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux specific) */
};

int main()
{
	key_t semkey = ftok( "/tmp/sem.temp", 1 ); 
	int semd = semget( semkey, 16, IPC_CREAT | 0666 );
	unsigned short semaphores[16];
	for( int i = 0; i < 16; i++ ) {
		semaphores[i] = i;
	}
	semun sun;
	sun.array = semaphores;
	semctl( semd, 0, SETALL, sun );
	return 0;
}
