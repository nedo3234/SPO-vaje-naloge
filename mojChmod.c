#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define err_sys(msg) { fprintf(stderr, "Error: %s.\n", msg); exit(1); }

int main(int argc, char *argv[]){
    if(argc != 3)
        err_sys("invalid number of args");
    
    
    struct stat buf;
    if(stat(argv[2], &buf) < 0)
        err_sys("stat error");

    mode_t mode = buf.st_mode & 07777;
    char *ep;
    long int n = strtol(argv[1], &ep, 8);

    if(*ep == '\0' && ep != argv[1]){
        mode = n;
    }
    else if(strcmp(argv[1], "u+s") == 0){
        mode |= S_ISUID;
    }
    else if(strcmp(argv[1], "u-s") == 0){
        mode &= ~S_ISUID;
    }
    else if(strcmp(argv[1], "g+s") == 0){
        mode |= S_ISGID;
    }
    else if(strcmp(argv[1], "a+rX") == 0){
        if(buf.st_mode & (S_IRUSR | S_IRGRP | S_IROTH))
            mode |= S_IRUSR | S_IRGRP | S_IROTH;
    }
    else{
        err_sys("invalid usage of args");
    }
    if(chmod(argv[2], mode) < 0)
        err_sys("chmod error");

    if(stat(argv[2], &buf) < 0)
        err_sys("stat error");

    printf("Nova dovoljenja: ");
    printf((buf.st_mode & S_IRUSR) ? "r" : "-");
    printf((buf.st_mode & S_IWUSR) ? "w" : "-");
    if(buf.st_mode & S_ISUID){
        printf((buf.st_mode & S_IXUSR) ? "s" : "S");
    }
    else{
        printf((buf.st_mode & S_IXUSR) ? "x" : "-");
    }
    
    printf((buf.st_mode & S_IRGRP) ? "r" : "-");
    printf((buf.st_mode & S_IWGRP) ? "w" : "-");
    if(buf.st_mode & S_ISGID){
        printf((buf.st_mode & S_IXGRP) ? "s" : "S");
    }
    else{
        printf((buf.st_mode & S_IXGRP) ? "x" : "-");
    }
        
    printf((buf.st_mode & S_IROTH) ? "r" : "-");
    printf((buf.st_mode & S_IWOTH) ? "w" : "-");
    if(buf.st_mode & S_ISVTX){
        printf((buf.st_mode & S_IXOTH) ? "t" : "T");
    }
    else{
        printf((buf.st_mode & S_IXOTH) ? "x" : "-");
    }

    printf("\n");
    return 0;
    
}