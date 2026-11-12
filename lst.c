#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

#define err(mess) { fprintf(stderr, "Error: %s.\n", mess); exit(1); }

int primerjajCase(const void *a, const void *b) {
    const char *datoteka1 = *(const char **)a;
    const char *datoteka2 = *(const char **)b;

    struct stat st1, st2;

    if(stat(datoteka1, &st1) != 0)
        return 0;
    
    if(stat(datoteka2, &st2) != 0)
        return 0;

    if(st1.st_mtime < st2.st_mtime) return 1;
    if(st1.st_mtime > st2.st_mtime) return -1;
    return 0;  
}

int primerjajCaseR(const void *a, const void *b) {
    const char *datoteka1 = *(const char **)a;
    const char *datoteka2 = *(const char **)b;

    struct stat st1, st2;

    if(stat(datoteka1, &st1) != 0)
        return 0;
    
    if(stat(datoteka2, &st2) != 0)
        return 0;

    if(st1.st_mtime < st2.st_mtime) return -1;
    if(st1.st_mtime > st2.st_mtime) return 1;
    return 0;  
}

int main(int argc, char *argv[]){
    DIR *dp;
    struct dirent *dirp;
    char *dir;
    bool reverse;
    if(argc == 2){
        dir = argv[1];
        reverse = false;
    }
    else if (argc==3 && (strcmp(argv[1], "-r")) == 0){
        dir = argv[2];
        reverse = true;
    }
    else
        err("invalid args");


    if((dp=opendir(dir)) == NULL)
        err("Can't open.");

    int st=0;    
    while((dirp = readdir(dp)) != NULL)
        st++;

    char *datoteke[st];
    rewinddir(dp);
    int i=0;
    while((dirp = readdir(dp)) != NULL){
        datoteke[i] = dirp->d_name;
        i++;
    }
    closedir(dp);

    reverse ? qsort(datoteke, st, sizeof(char *), primerjajCaseR) : qsort(datoteke, st, sizeof(char *), primerjajCase);
    for(i=0; i < st; i++){
        printf("%d. %s\n", i+1, datoteke[i]);
    }

    return 0;
}