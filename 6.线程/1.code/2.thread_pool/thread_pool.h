/*************************************************************************
	> File Name: thread_pool.h
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Thu 21 Jan 2021 08:39:37 PM CST
 ************************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
struct task_queue {
    int size;
    int total;
    int head;
    int tail;
    char **data;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

void task_queue_init(struct task_queue *taskQueue, int size);
void task_queue_push(struct task_queue *taskQueue, char *str);
char *task_queue_pop(struct task_queue *taskQueue);
#endif
