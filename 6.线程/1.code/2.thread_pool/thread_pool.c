/*************************************************************************
	> File Name: thread_pool.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Thu 21 Jan 2021 08:44:56 PM CST
 ************************************************************************/

#include "head.h"
#include "thread_pool.h"

void task_queue_init(struct task_queue *task_Queue, int size) {
    task_Queue -> size = size;
    task_Queue -> total = 0;
    task_Queue -> head = task_Queue -> tail = 0;
    pthread_mutex_init(&task_Queue->mutex,NULL);
    pthread_cond_init(&task_Queue->cond, NULL);
    task_Queue -> data = calloc(size, sizeof(char *));
    return;
}


void task_queue_push(struct task_queue *task_Queue, char *str) {
    pthread_mutex_lock(&task_Queue -> mutex);
    if (task_Queue -> total == task_Queue -> size) {
        pthread_mutex_unlock(&task_Queue->mutex);
        printf("task queue is fukk !\n");
        return;
    }
    printf("<push> : %s\n",str);
    task_Queue->data[task_Queue->tail++] = str;
    task_Queue->total++;
    if (task_Queue->tail == task_Queue->size) {
        printf("task queue reach end!\n");
        task_Queue->tail = 0;
    }
    pthread_cond_signal(&task_Queue->cond);
    pthread_mutex_unlock(&task_Queue->mutex);
}


char *task_queue_pop(struct task_queue *task_Queue) {
    pthread_mutex_lock(&task_Queue->mutex);
    while (task_Queue->total == 0) {
        printf("task_queue is empty!\n");
        pthread_cond_wait(&task_Queue->cond, &task_Queue->mutex);
    }
    char *str = task_Queue->data[task_Queue->head];
    printf("<pop> : %s\n", str);
    task_Queue->total--;
    if (++task_Queue->head == task_Queue->size) {
        printf("task queue reach end!\n");
        task_Queue -> head = 0;
    }
    pthread_mutex_unlock(&task_Queue->mutex);
    return str;
}
