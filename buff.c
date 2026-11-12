#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define err(mess) { fprintf(stderr, "Error: %s\n", mess); exit(1); }
#define BUFSIZE 4096

int main(int argc, char *argv[]) {
    FILE *fp;
    char buf[BUFSIZE];
    struct timespec start, end;

    if(argc != 3)
        err("invalid args");

    long B = strtol(argv[1], NULL, 10);
    long N = strtol(argv[2], NULL, 10);

    if(B <= 0 || B > BUFSIZE)
        err("B mora biti med 1 in velikostjo medpomnilnika");
    
    if(N <= 0)
        err("N rabi biti pozitiven");
    
    if((fp = fopen("izhod.txt", "w")) == NULL)
        err("fopen error");

    if(setvbuf(fp, buf, _IOFBF, B))
        err("setvbuf error");

    if(clock_gettime(CLOCK_MONOTONIC, &start) == -1)
        err("clock_gettime error");

    for(int i=0; i < N; i++){
        if(fputs("Nema odmora dok traje obnova!\n", fp) == EOF)
            err("fputs error");
    }

    if(fclose(fp) == EOF)
        err("fclose error");

    if(clock_gettime(CLOCK_MONOTONIC, &end) == -1)
        err("clock_gettime error");

    

    double casIzvajanja = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("B = %ld, N = %ld, čas izvajanja: %f sekund\n", B, N, casIzvajanja);

    return 0;
}