/*************************************************************************
	> File Name: 1.sleep.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Tue 23 Mar 2021 10:23:53 PM CST
 ************************************************************************/

#include "head.h"
void wakeup() {
    printf("wakeup\n");
}

int main (int argc, char **argv) {
    signal(SIGALRM,wakeup);
    int time = atoi(argv[1]);
    alarm(time);
    pause();
    printf("After pause\n");
    return 0;
}
