/*************************************************************************
	> File Name: 1.sem.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Thu 18 Mar 2021 07:16:56 PM CST
 ************************************************************************/

#include "head.h"

#define VALUE 3
union semun {
    int val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *_buf;  /* Buffer for IPC_INFO
                                               (Linux-specific) */
};



int create_sem() {
    key_t key = ftok(".",2021);
    int semid;
    if ((semid = semget(key, 1, IPC_CREAT | 0666)) < 0) {
        return -1;
    }
    return semid;
}


int init_sem(int sem_id) {
    union semun arg;
    arg.val = VALUE;
    return semctl(sem_id, 0, SETVAL, arg);
}

int sem_P(int sem_id) {
    struct sembuf sbuff;
    sbuff.sem_num = 0;
    sbuff.sem_op = -1;
    sbuff.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sbuff, 1) == -1) {
        perror("shmop");
        return -1;
    }
    return 0;
}

int sem_V(int sem_id) {
    struct sembuf sbuff;
    sbuff.sem_num = 0;
    sbuff.sem_op = 1;
    sbuff.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sbuff, 1) == -1) {
        perror("shmop");
        return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
    int sem_id;
    if ((sem_id = create_sem()) < 0) {
        perror("create_sem");
        exit(1);
    }
    if (argc > 1) {
        if (init_sem(sem_id) < 0) {
            perror("init_sem");
            exit(1);
        }
    }
    int cnt = 0;
    while (1) {
        sem_P(sem_id);
        cnt++;
        sem_V(sem_id);
        printf("cnt = %d\n", cnt);
        sleep(5);
    }
}
