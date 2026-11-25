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

void izvedi(char *args[]) {
    execvp(args[0], args);
    fprintf(stderr, "couldn't execute: %s\n", args[0]);
    exit(127);
}

int main(void) {
    int status;
    char buf[MAXSIZE];
    char *args1[32];
    char *args2[32];
    int st = 0;
    printf("%% ");
    while(fgets(buf, MAXSIZE, stdin) != NULL) {
        buf[strlen(buf) - 1] = 0;
        char *desna = strstr(buf, "=>");

        if(desna == NULL) {
            st % 2 == 0 ? parse(buf, args1) : druga(buf, args1);
            if(args1[0] != NULL) {
                pid_t pid;
                if((pid = fork()) < 0)
                    err("fork error");
                if(pid == 0)
                    izvedi(args1);
                if(waitpid(pid, &status, 0) < 0)
                    err("waitpid error")
            }
        }
        else {
            *desna = '\0';
            desna += 2;
            parse(buf, args1);
            parse(desna, args2);

            if(args1[0] == NULL || args2[0] == NULL)
                fprintf(stderr, "Error: missing command\n");
            else {
                int fd[2];
                pid_t p2, p3;

                if(pipe(fd) < 0)
                    err("pipe error");
                
                if((p2 = fork()) < 0)
                    err("fork error")
                if(p2 == 0) {
                    close(fd[0]);
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[1]);
                    izvedi(args1);
                }

                if((p3 = fork()) < 0)
                    err("fork error")
                if(p3 == 0) {
                    close(fd[1]);
                    dup2(fd[0], STDIN_FILENO);
                    close(fd[0]);
                    izvedi(args2);
                }

                close(fd[0]);
                close(fd[1]);
                if(waitpid(p2, &status, 0) < 0)
                    err("waitpid error")
                if(waitpid(p3, &status, 0) < 0)
                    err("waitpid error")
            }
        }


        st++;
        printf("%% ");
    }

    exit(0);
}