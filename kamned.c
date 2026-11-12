#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define err(mess) { fprintf(stderr, "Error: %s\n", mess); exit(1); }
#define BUFFSIZE 4096

int main(int argc, char *argv[]) {
    FILE *fp;
    char buf[BUFFSIZE];

    if(argc < 3 || argc > 4)
        err("invalid number of args")
    else if(argc == 3 && strcmp("-a", argv[1]) == 0) {
        if((fp = fopen(argv[2], "a")) == NULL)
            err("fopen error");
        while(fgets(buf, BUFFSIZE, stdin) != NULL) {
            if(fputs(buf, fp) == EOF)
                err("fputs error");
        }
        if(ferror(stdin))
            err("input error");
        if(fclose(fp) == EOF)
            err("fclose error");
    }
    else if(argc == 4 && strcmp("-d", argv[1]) == 0) {
        int n = atoi(argv[2]);
        if(n <= 0)
            err("invalid n");
        FILE *temp;
        if((fp = fopen(argv[3], "r")) == NULL)
            err("fopen error");
        if((temp = fopen("temp", "w")) == NULL)
            err("fopen error");
        int i=1;
        while(fgets(buf, BUFFSIZE, fp) != NULL) {
            if(i == n) {
                i++;
                continue;
            }
            else {
                if(fputs(buf, temp) == EOF)
                    err("fputs error");
                i++;
            }
        }
        if(fclose(fp) == EOF || fclose(temp) == EOF)
            err("fclose error");
        
        if(unlink(argv[3]) < 0)
            err("unlink error");
        
        if(rename("temp", argv[3]) < 0)
            err("rename error");

    }
    else if(argc == 4 && strcmp("-i", argv[1]) == 0) {
        int n = atoi(argv[2]);
        if(n <= 0)
            err("invalid n");

        FILE *temp;
        
        if((fp = fopen(argv[3], "r")) == NULL)
            err("fopen error");
        if((temp = fopen("temp", "w")) == NULL)
            err("fopen error");
        
        char vnos[BUFFSIZE];
        int i=1, vstavljeno=0;
        while(fgets(buf, BUFFSIZE, fp) != NULL) {
            if(i == n) {
                if(fgets(vnos, BUFFSIZE, stdin) != NULL) {
                    if(fputs(vnos, temp) == EOF)
                        err("fputs error");
                }
                vstavljeno = 1;
            }
            if(fputs(buf, temp) == EOF)
                err("fputs error");
            i++;
        }
        if(ferror(fp))
            err("fgets error");

        if(!vstavljeno) {
            if(fgets(vnos, BUFFSIZE, stdin) != NULL) {
                if(fputs(vnos, temp) == EOF)
                    err("fputs error");
            }
        }

        if(fclose(fp) == EOF || fclose(temp) == EOF)
            err("fclose error");
        
        if(unlink(argv[3]) < 0)
            err("unlink error");
        
        if(rename("temp", argv[3]) < 0)
            err("rename error");
    }
    else
        err("invalid usage of args");

    return 0;
}