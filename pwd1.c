#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>



#define err(mess) { fprintf(stderr, "Error: %s.\n", mess); exit(1); }
#define BUFSIZE 500

int main(void){
    char buf[BUFSIZE];
    struct stat tren, star;
    struct dirent *dirp;  
    size_t pos = BUFSIZE - 1;  
    DIR *dp;
    int izvor;

    if((izvor = open(".", O_RDONLY)) < 0)
        err("open error");

    buf[pos] = '\0';

    while(1) {
        if(stat(".", &tren) < 0 || stat("..", &star) < 0)
            err("stat error");

        if(tren.st_ino == star.st_ino && tren.st_dev == star.st_dev)
            break;
        
        if(chdir("..") < 0)
            err("chdir error");

        if((dp = opendir(".")) == NULL)
            err("opendir error");

        int najden = 0;
        while((dirp = readdir(dp)) != NULL){
            if(dirp->d_ino == tren.st_ino) {
                size_t len = strlen(dirp->d_name);
                if(len + 1 > pos)
                    err("buffer too small");

                pos -= len;
                memcpy(buf+pos, dirp->d_name, len);
                buf[--pos] = '/';
                najden = 1;
                break;
            }
        }
        closedir(dp);
        
        if(!najden)
            err("cannot find directory");
    }

    if(buf[pos] == '\0')
        buf[--pos] = '/';

    memmove(buf, buf+pos, BUFSIZE-pos);
    fchdir(izvor);
    close(izvor);

    printf("%s\n", buf);
    return 0;
}