#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define err_sys(mess) { fprintf(stderr, "Error: %s\n", mess); exit(1); }

volatile sig_atomic_t t = 0;

void alarmHandler(int sig) {
    t = 1;
}
static void sig_usr(int);



int main(void) {
    if(signal(SIGALRM, alarmHandler) == SIG_ERR)
        err_sys("ne morem loviti SIGALRM")
    
    printf("Lovim signal SIGINT ...\n");

    if(signal(SIGINT, sig_usr) == SIG_ERR)
        err_sys("ne morem loviti SIGINT")
    t = 0;
    alarm(5);
    while(!t) {
        pause();
    }

    printf("Alarm v casu t=5 ...\n");
    if(signal(SIGINT, SIG_IGN) == SIG_ERR)
        err_sys("ne morem loviti SIGINT")
    printf("Ignoriram signal SIGINT ...\n");
    t = 0;
    alarm(5);
    while(!t) {
        pause();
    }
    printf("Alarm v casu t=10 ...\n");
    if(signal(SIGINT, SIG_DFL) == SIG_ERR)
        err_sys("ne morem loviti signal SIGINT")
    
    printf("Privzeta akcija za signal SIGINT ...\n");
    t = 0;
    alarm(5);
    while(!t) {
        pause();
    }
    printf("Alarm v casu t=15 ...\n");
    return 0;
}

static void sig_usr(int signo) {
    if(signo == SIGINT)
        printf("Prejel sem signal %d.\n", signo);
    else
        return;
}