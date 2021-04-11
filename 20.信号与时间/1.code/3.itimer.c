/*************************************************************************
	> File Name: 3.itimer.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Tue 23 Mar 2021 11:40:48 PM CST
 ************************************************************************/

#include "head.h"

void print(int sig) {
    printf("3 sec counts down.\n");
}
int main () {

    struct itimerval itv;
    itv.it_interval.tv_sec = 3;
    itv.it_interval.tv_usec = 0;

    itv.it_value.tv_sec = 10;
    itv.it_value.tv_usec = 0;
    signal(SIGALRM, print);
    setitimer(ITIMER_REAL, &itv, NULL);

    while (1) {
        pause();
    }

    return 0;
}
