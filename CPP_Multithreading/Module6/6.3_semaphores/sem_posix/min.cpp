#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    sem_t *sem;
    sem = sem_open( "/test.sem", O_CREAT, 0777, 66 );
    if( sem == SEM_FAILED ) {
        perror( "Cannot open semaphore" );
        exit( -1 );
    }

    pause();

    return 0;
}
