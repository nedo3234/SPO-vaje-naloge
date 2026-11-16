#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define err(mess) { fprintf(stderr, "Error: %s\n", mess); exit(1);}
#define MAXSIZE 64

void parse(char *niz_ukaz, char *args[]) {
    int n = 0;
    int v_besedi = 0;

    for(int i=0; niz_ukaz[i] != '\0'; i++) {
        if(niz_ukaz[i] == ' ') {
            niz_ukaz[i] = '\0';
            v_besedi = 0;
        } else if(!v_besedi) {
            args[n++] = &niz_ukaz[i];
            v_besedi = 1;
        }
    }
    args[n] = NULL;

}

void druga(char *niz_ukaz, char *args[]) {
    int i = 0;
    char *token = strtok(niz_ukaz, " ");
    while(token != NULL && i < 31) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

int main(void) {
    pid_t pid;
    int status;
    char buf[MAXSIZE];
    char *args[32];
    int st = 0;
    printf("%% ");
    while(fgets(buf, MAXSIZE, stdin) != NULL) {
        buf[strlen(buf) - 1] = 0;
        if((pid = fork()) < 0) {
            err("fork error");
        }
        else if(pid == 0) {
            st % 2 == 0 ? parse(buf, args) : druga(buf, args);
            execvp(args[0], args);
            printf("couldn't execute: %s\n", buf);
            exit(127);
        }

        if((pid = waitpid(pid, &status, 0)) < 0)
            err("waitpid error");

        st++;
        printf("%% ");
    }

    exit(0);
}