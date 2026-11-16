#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define err(mess) { fprintf(stderr, "Error: %s\n", mess); exit(1);}
#define MAXSIZE 64

int main(void) {
    pid_t pid;
    int status;
    char buf[MAXSIZE];

    printf("%% ");
    while(fgets(buf, MAXSIZE, stdin) != NULL) {
        buf[strlen(buf) - 1] = 0;
        if((pid = fork()) < 0) {
            err("fork error");
        }
        else if(pid == 0) {
            execlp(buf, buf, (char *) 0);
            printf("couldn't execute: %s\n", buf);
            exit(127);
        }

        if((pid = waitpid(pid, &status, 0)) < 0)
            err("waitpid error");

        printf("%% ");
    }
    exit(0);
}