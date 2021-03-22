/*************************************************************************
	> File Name: 1.server.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Thu 21 Jan 2021 06:39:40 PM CST
 ************************************************************************/

#include "head.h"
#include "chat.h"

struct Msg *share_memory = NULL;

int main () {

    int shmid;
    key_t key = ftok(".",202101);

    if ((shmid = shmget(key,sizeof(struct Msg), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((share_memory = (struct Msg *)shmat(shmid, NULL, 0)) < 0) {
        perror("shmat");
        exit(1);
    }
    memset(share_memory, 0, sizeof(struct Msg));
    pthread_mutexattr_t m_attr;
    pthread_mutexattr_init(&m_attr);
    pthread_mutexattr_setpshared(&m_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&share_memory->mutex, &m_attr);
   
    pthread_condattr_t c_attr;
    pthread_condattr_init(&c_attr);
    pthread_condattr_setpshared(&c_attr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&share_memory->cond, &c_attr);
    
    while(1) {
        pthread_mutex_lock(&share_memory -> mutex);
        printf("Server Get the Mutex!\n");
        pthread_cond_wait(&share_memory->cond, &share_memory->mutex);
        printf("Server got the cond signal!\n");
        printf("<%s> : %s.\n",share_memory->name, share_memory->msg);
        memset(share_memory->msg, 0, sizeof(share_memory->msg));
        pthread_mutex_unlock(&share_memory->mutex);
    }
}
