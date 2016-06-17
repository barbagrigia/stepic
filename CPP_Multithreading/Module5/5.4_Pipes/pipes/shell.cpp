#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>
#include <iterator>
#include <algorithm>

using namespace std;

int execute_child( const vector<string>& path, int pipe_input = -1 )
{
	int pfd[2];
	pipe( pfd );
	

	int child_pid = fork();
	if( !child_pid ) {
		char*  argv[64];
		int i = 0;
		for( unsigned int i =0 ; i < path.size(); i++ ) {
			argv[i] = const_cast<char*>( path[i].c_str() );
		}
		argv[i] = 0;
	
		if( pipe_input != -1 ) {		
			close( STDIN_FILENO );
			dup2( pipe_input, STDIN_FILENO );
			close( pipe_input );
		}
		close( STDOUT_FILENO );
		dup2( pfd[1], STDOUT_FILENO );
		close( pfd[0] );
		close( pfd[1] );
		execvp( path[0].c_str(), argv );
		exit( 0 );
		return -1;
	} else {
		close( pfd[1] );
		waitpid( child_pid, 0, 0 );
		return pfd[0];
	}
}

void out_result( int pipe_input )
{
	char buffer[4096];
	size_t len = 0;
	FILE* file = fopen( "/home/box/result.out", "w" );
	while( (len = read( pipe_input, buffer, sizeof( buffer ) ) ) > 0  ) {
		size_t len2 = fwrite( buffer, 1, len, file );
		if( len2 < len ) {
			printf( "Copy error\n" );
			return;
		}
	}
}

int main(int, char** )
{
	string input;
	getline( cin, input );

	int pipe_input = -1;	
	unsigned int pipe_pos = 0;
	while( pipe_pos != string::npos ) {
		string command;
		pipe_pos = input.find( '|' );
		if( pipe_pos == string::npos ) {
			command = input;
		} else {
			command = input.substr( 0, pipe_pos );
			input = input.substr( pipe_pos + 1 );
		}
		istringstream iss( command );
		vector<string> commands( istream_iterator<string>{iss},
					istream_iterator<string>{} );
		pipe_input = execute_child( commands, pipe_input );
	}
	
	out_result( pipe_input );		
	close( pipe_input );
	return 0;
}
