#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
	int shmid = shm_open( "/test.shm", O_CREAT | O_RDWR | O_TRUNC, 0666 );
	ftruncate( shmid, 1024*1024 );
	char* attached = (char*)mmap( 0, 1024*1024, PROT_WRITE, MAP_SHARED, shmid, 0 );

	for( int i = 0; i < 1024*1024; i++ ) {
		attached[i]	 = 13;
	}
	munmap( attached, 1024*1024 );
	close( shmid );
	return 0;
}
