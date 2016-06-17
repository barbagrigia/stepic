#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/socket.h>

void child( int socket ) {
    const char hello[] = "hello parent, I am child";
    while ( true ) {
      write( socket, hello, sizeof( hello ) ); /* NB. this includes nul */
    }
    /* go forth and do childish things with this end of the pipe */
    close( socket );
}

void parent( int socket ) {
    /* do parental things with this end, like reading the child's message */
    char buf[1024];
    while ( true ) {
      int n = read( socket, buf, sizeof( buf ) );
      if (n > 0)
        printf( "parent received '%.*s'\n", n, buf );
    }
    close( socket );
}

void socketfork() {

}

void test_socketpair() {
  int fd[2];
  static const int parentsocket = 0;
  static const int childsocket = 1;
  pid_t pid;

  socketpair( PF_LOCAL, SOCK_STREAM, 0, fd );

  pid = fork();
  if ( pid == 0 ) { // child
      close( fd[parentsocket] ); /* Close the parent file descriptor */
      child( fd[childsocket] );
  } else {    // parent
      close( fd[childsocket] ); /* Close the child file descriptor */
      parent( fd[parentsocket] );
  }
  exit( 0 ); /* do everything in the parent and child functions */
}

int main( int argc, char* argv[] )
{
    test_socketpair();
    return 0;
}
