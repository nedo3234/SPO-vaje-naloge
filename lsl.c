#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>

#define err_sys(msg) { fprintf(stderr, "Error: %s.\n", msg); exit(1);}

int main(int argc, char *argv[]){
    int i;
    for(i=1; i < argc; i++)
    {
        struct stat buf;
        if(stat(argv[i], &buf) < 0)
            err_sys("stat error");

        char tip;
        struct passwd *pw = getpwuid(buf.st_uid);

        int stPovezav = buf.st_nlink;
        long velikost = buf.st_size;
        if(S_ISREG(buf.st_mode))    tip='-';
        else if(S_ISDIR(buf.st_mode))    tip='d';
        else if(S_ISCHR(buf.st_mode))    tip='c';
        else if(S_ISBLK(buf.st_mode))    tip='b';
        else if(S_ISFIFO(buf.st_mode))    tip='p';
        else if(S_ISLNK(buf.st_mode))    tip='l';
        else if(S_ISSOCK(buf.st_mode))    tip='s';
        else    err_sys("unkown type");

        printf("%c", tip);
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
        if(pw != NULL)
            printf(" %d %s %ld %s\n", stPovezav, pw->pw_name, velikost, argv[i]);
        else    
            printf(" %d %u %ld %s\n", stPovezav, (unsigned)buf.st_uid,  velikost, argv[i]);
    }
    

    return 0;
    
}