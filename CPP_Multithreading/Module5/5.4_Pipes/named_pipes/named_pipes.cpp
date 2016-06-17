#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE];

using namespace std;

int main(int argc, char **argv) {

	const char *read_pipe_name = "in.fifo";
	const char *write_pipe_name = "out.fifo";

	mkfifo(read_pipe_name, 0777);
	mkfifo(write_pipe_name, 0777);

	//read pipe open in non-block
	int read_pipe = open(read_pipe_name, O_NONBLOCK | O_RDONLY);
	if (read_pipe < 0) {
		cerr << "Can't open read pipe\n";
		return 1;
	}

	cerr << "Read pipe opened\n";

	//write pipe. Wait until reader is connected
	int write_pipe = open(write_pipe_name, O_WRONLY);
	if (write_pipe < 0) {
		cerr << "Can't open write pipe\n";
		return 1;
	}
	
	cerr << "Write pipe opened\n";

	for(;;) {
		int bytes_readed = 0;
		while (bytes_readed <= 0)
			bytes_readed = read(read_pipe, buffer, BUFFER_SIZE);
		
		cerr << "Readed " << bytes_readed << endl;

		int bytes_writen = 0;
		bytes_writen = write(write_pipe, buffer, bytes_readed);
		if (bytes_writen < bytes_readed) {
			cerr << "Some bytes not writen\n";
		}

		cerr << "Writen" << bytes_writen << endl;
	}

	return 0;	
}
