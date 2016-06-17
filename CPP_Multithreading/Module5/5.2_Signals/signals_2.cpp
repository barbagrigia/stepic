#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>
#include <string.h>

volatile sig_atomic_t done = 0;

void term_handler(int signum) {
        done = 1;
}

void child_handler(int signum) {
        pid_t pid;
        int status;
        while((pid = waitpid(-1, &status, WNOHANG)) > 0);
}

int main() {
        pid_t pid_parent = getpid();
        FILE *f = fopen("/home/box/pid_parent", "w");
        fprintf(f, "%d", pid_parent);
        fclose(f);

        pid_t pid_child;

        if(pid_child = fork()) {
                FILE *f = fopen("/home/box/pid_child", "w");
                fprintf(f, "%d", pid_child);
                fclose(f);
                
                struct sigaction action;
                memset(&action, 0, sizeof(struct sigaction));
                action.sa_handler = child_handler;
                sigaction(SIGCHLD, &action, NULL);
        } else {
                struct sigaction action;
                memset(&action, 0, sizeof(struct sigaction));
                action.sa_handler = term_handler;
                sigaction(SIGTERM, &action, NULL);
        }
 
        while(!done) {
                sleep(1);
        }
}
