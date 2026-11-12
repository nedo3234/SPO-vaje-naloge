#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define err_sys(mess) {fprintf(stderr, "Error: %s\n", mess); exit(1);}
#define BUFFSIZE 8192

int main(int argc, char *argv[]){
    if(argc < 2)
        err_sys("too few args");

    int flags = O_WRONLY | O_CREAT | O_TRUNC;
    char *file;
    int n;  
    char buf[BUFFSIZE];
    int fd;

    if(argc == 2)
        file = argv[1];
    else if(argc == 3 && strcmp(argv[1], "-a") == 0)
    {
        file = argv[2];
        flags = O_WRONLY | O_CREAT | O_APPEND;
    }
    else
        err_sys("usage of args");    



    if((fd = open(file, flags, 0666)) < 0)
        err_sys("file opening");


    while((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0){
        if(write(STDOUT_FILENO, buf, n) != n)
            err_sys("standard output writing");
        if(write(fd, buf, n) != n)
            err_sys("file writing");    
    }

    if(n < 0)
        err_sys("file reading");

    if(close(fd) < 0)
        err_sys("file closing");    
    return 0;
}