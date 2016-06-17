#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


void pipe_cmd() {
  char path[] = "//home//box//result.out";
  char command[255] = {0};
  char result[255] = {0};
  if ( fgets( command, sizeof( command ), stdin ) )
  {
    FILE* fexec = popen( command, "r" );
    FILE* fout = fopen( path, "w" );
    int len = fread( result, 1, sizeof( result ), fexec );
    int res = fwrite( result, 1, len, fout );
    pclose( fexec );
    fclose( fout );
  }
}

int main( int argc, char **argv ) {
  pipe_cmd();
  
  return 0;
}