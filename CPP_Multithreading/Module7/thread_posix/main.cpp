#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

void write_my_pid()
{
	FILE* out = fopen( "/home/box/pid", "w" );
	fprintf( out, "%d", getpid() );
	fflush( out );
	fclose( out );
}

void* thread_entry( void* arg )
{
	sleep( 30 );
	return 0;
}

void run_and_wait_thread()
{
	pthread_t thread;
	pthread_create( &thread, 0, thread_entry, 0 );
	pthread_join( thread, 0 );
}


int main()
{
	write_my_pid();	
	run_and_wait_thread();

	return 0;
}
