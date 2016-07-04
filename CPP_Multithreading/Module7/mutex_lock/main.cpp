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

void* wait_mutex( void* arg )
{
	pthread_mutex_t* mut = (pthread_mutex_t*)arg;
	pthread_mutex_lock( mut );
	pthread_mutex_unlock( mut );	
	return 0;
}

void* wait_spinlock( void* arg )
{
	pthread_spinlock_t* spin = (pthread_spinlock_t*)arg;
	pthread_spin_lock( spin );
	pthread_spin_unlock( spin );
	return 0;
}

void* wait_read_lock( void* arg )
{
	pthread_rwlock_t* lock = (pthread_rwlock_t*)arg;
	pthread_rwlock_rdlock( lock );
	pthread_rwlock_unlock( lock );	
	return 0;
}

void* wait_write_lock( void* arg )
{
	pthread_rwlock_t* lock = (pthread_rwlock_t*)arg;
	pthread_rwlock_wrlock( lock );
	pthread_rwlock_unlock( lock );	
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
	run( wait_mutex, &mut );

	pthread_spinlock_t spin;
	pthread_spin_init( &spin, 0 );
	pthread_spin_lock( &spin );
	run( wait_spinlock, (void*)&spin );

	pthread_rwlock_t rwlock;
	pthread_rwlock_init( &rwlock, 0 );
	pthread_rwlock_wrlock( &rwlock );
	run( wait_read_lock, &rwlock );
	run( wait_write_lock, &rwlock );
	
	sleep( 10 );
	pthread_mutex_unlock( &mut );
	pthread_spin_unlock( &spin );
	pthread_rwlock_unlock( &rwlock );

	sleep( 1 );

	return 0;
}
