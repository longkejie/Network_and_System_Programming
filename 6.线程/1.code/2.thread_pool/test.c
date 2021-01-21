/*************************************************************************
	> File Name: test.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Thu 21 Jan 2021 09:02:34 PM CST
 ************************************************************************/

#include "head.h"
#include "thread_pool.h"

#define THREAD 5
#define QUEUE 50

void *do_work(void *arg) {
    pthread_detach(pthread_self());
    struct task_queue *taskQueue = (struct task_queue *)arg;
    while(1) {
        char *str = task_queue_pop(taskQueue);
        printf("<%d> : %s !\n",pthread_self(),str);
    }
}

int main () {

    pthread_t thread[THREAD];
    struct task_queue taskQueue;
    task_queue_init(&taskQueue, QUEUE);
    char buff[QUEUE][1024] = {0};
    int sub = 0;
    for (int i = 0; i < THREAD; ++i) {
        pthread_create(&thread[i], NULL,do_work, (void *)&taskQueue);
    }
    
    while (1) {
        FILE *fp = fopen("./test.c","r");
        if (fp == NULL) {
            perror("fopen");
            exit(1);
        }
        while (fgets(buff[sub++],1024,fp) != NULL) {
            task_queue_push(&taskQueue, buff[sub]);
            if (sub == QUEUE) {
                sub = 0;
            }
            if (taskQueue.total == taskQueue.size) {
                while (1) {
                    if (taskQueue.total < taskQueue.size) {
                        break;
                    }
                    usleep(10000);;
                }
            }
        }
        fclose(fp);
    }

}
