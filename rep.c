#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
// rep –n 3 a.txt

#define err_sys(mess) { write(STDERR_FILENO, "Error: ", 7); \
                        write(STDERR_FILENO, mess, strlen(mess)); \
                        write(STDERR_FILENO, "\n", 1); \
                        exit(1); }
#define BUFFSIZE 8192

int main(int argc, char *argv[]){
    if (argc < 2)
        err_sys("too few args");

    int fd;
    int n=5;
    char *file;
    char buf[BUFFSIZE];
    char c;

    if(argc == 2)
        file = argv[1];
    else if(argc == 4 && strcmp(argv[1], "-n") == 0){
        file = argv[3];
        n = atoi(argv[2]);
    }
    else
        err_sys("usage of args");    

    if (n <= 0)
        err_sys("invalid n");

    if((fd = open(file, O_RDONLY)) < 0)
        err_sys("file opening");

    off_t pos;    

    if((pos = lseek(fd, 0, SEEK_END)) == -1)
        err_sys("lseek");

    if(pos > 0){
        if(lseek(fd, pos-1, SEEK_SET) == -1)
            err_sys("lseek");
        if(read(fd, &c, 1) != 1)
            err_sys("reading");
        if(c == '\n')
            pos--;        
    }    

    off_t start = 0;
    while(pos > 0){
        pos--;
        if(lseek(fd, pos, SEEK_SET) == -1)
            err_sys("lseek");
        if(read(fd, &c, 1) != 1)
            err_sys("reading");
        if(c == '\n' && --n == 0){
            start = pos + 1;
            break;
        }        
    }

    if(lseek(fd, start, SEEK_SET) == -1)
        err_sys("lseek");

    int r;
    while((r = read(fd, buf, BUFFSIZE)) > 0){
        if(write(STDOUT_FILENO, buf, r) != r)
            err_sys("standard output writing");
    }
    if(r < 0)
        err_sys("reading");    

    if(close(fd) < 0)
        err_sys("file closing");    
    return 0;
}