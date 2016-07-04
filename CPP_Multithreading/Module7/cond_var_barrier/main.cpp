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

struct cond_mutex_t {
	pthread_cond_t* cond;
	pthread_mutex_t* mut;
};

void* wait_cond( void* arg )
{
	cond_mutex_t* pair = (cond_mutex_t*)arg;
	pthread_cond_wait( pair->cond, pair->mut );
	return 0;
}

void* wait_barrier( void* arg )
{
	pthread_barrier_t* bar = (pthread_barrier_t*)arg;
	pthread_barrier_wait( bar );
	return 0;
}

typedef void* (*TEntry)( void* );

void run( TEntry entry_point, void* arg )
{
	pthread_t thread;

	pthread_create( &thread, 0, entry_point, arg );
	pthread_detach( thread );
	
}

int main()
{
	write_my_pid();	
	
	pthread_mutex_t mut;
	pthread_mutex_init( &mut, 0 );
	pthread_mutex_lock( &mut );
	pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	cond_mutex_t pair = { &cond, &mut };
	run( wait_cond, &pair );

	pthread_barrier_t barrier;
	pthread_barrier_init( &barrier, 0, 2 );
	run( wait_barrier, &barrier );

	sleep( 10 );
	
	pthread_cond_signal( &cond );
	pthread_barrier_wait( &barrier );

	sleep( 1 );

	pthread_barrier_destroy( &barrier );
	pthread_cond_destroy( &cond );
	pthread_mutex_destroy( &mut );

	return 0;
}
