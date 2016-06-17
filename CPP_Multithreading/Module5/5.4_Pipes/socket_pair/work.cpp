#include <sys/types.h>
#include <sys/socket.h>


void test_work() {
  int sv[2];
  int res = socketpair( AF_UNIX, SOCK_STREAM, 0, sv );
}

int main( int argc, char* argv[] ) {
    test_work();
    return 0;
}
