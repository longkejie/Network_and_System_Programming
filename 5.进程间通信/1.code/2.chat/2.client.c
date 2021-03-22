/*************************************************************************
	> File Name: 2.client.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Thu 21 Jan 2021 06:49:31 PM CST
 ************************************************************************/

#include "head.h"
#include "chat.h"

struct Msg *share_memory = NULL;


int main (int argc, char **argv) {
    int opt;
    char name[20] = {0};

    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch(opt) {
            case 'n':
                strcpy(name,optarg);
                break;
            default:
                fprintf(stderr,"Usage : %s -n name\n",argv[0]);
                exit(1);
        }
    }
    int shmid;
    key_t key = ftok(".",202101);
    if ((shmid = shmget(key, sizeof(share_memory), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((share_memory = (struct Msg *)shmat(shmid, NULL, 0)) < 0) {
        perror("shmat");
        exit(1);
    }

    while (1) {
        char msg[1024] = {0};
        scanf("%[^\n]s",msg);
        getchar();
        if (!strlen(msg)) continue;
        while (1) {
            if ((!strlen(share_memory->msg))) {
                pthread_mutex_lock(&share_memory->mutex);
                break;
            }
        }
        printf("Sending : %s...\n",msg);
        strcpy(share_memory->msg, msg);
        strcpy(share_memory->name,name);
        pthread_cond_signal(&share_memory->cond);
        pthread_mutex_unlock(&share_memory->mutex);
        printf("Client signaled the cond\n");
    }
    return 0;
}
