#include <stdio.h>                                                                                      
#include <stdlib.h>                                                                                     
#include <unistd.h>                                                                                     
#include <sys/types.h>                                                                                  
#include <signal.h>                                                                                     
#include <wait.h>                                                                                       
#include <string.h>                                                                                     

int main() { 
		pid_t pid = getpid();
		FILE *f = fopen("/home/box/pid", "w");
		fprintf(f, "%d", pid);
		fclose(f);
		
		signal(SIGTERM, SIG_IGN);
		signal(SIGINT, SIG_IGN);
		
		while(1) {
			pause();
        }
}
