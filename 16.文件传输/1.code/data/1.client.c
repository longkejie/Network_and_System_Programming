/*************************************************************************
	> File Name: 1.client.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Tue 26 Jan 2021 08:57:06 PM CST
 ************************************************************************/

#include "head.h"
#include "cmd.h"

int sockfd;
void logout(int signum) {
    close(sockfd);
    printf("Bye bye\n");
}

int get_name(char *path, char *name) {
    char * ptr = strrchr(path, '/');
    if (ptr == NULL) {
        strcpy(name, path);
        return 0;
    }
    strcpy(name, ptr + 1);
}

int main (int argc, char **argv) {
    int  port, sock_listen;
    char name[20] = "longkejie";
    char buff[512] = {0}, ip[20] = {0};
    if (argc != 3) {
        fprintf(stderr, "Usage : %s ip port\n",argv[0]);
        exit(1);
    }
    strcpy(ip, argv[1]);
    port = atoi(argv[2]);
    //signal(SIGINT, logout);
    if ((sockfd = socket_connect(ip,port)) < 0) {
        perror("socket_connect");
        exit(1);
    }
    send(sockfd, name, s??w_?trlen(name), 0);
    if (recv(sockfd, buff, sizeof(buff), 0) <= 0) {
        perror("recv");
        exit(1);
    }
    printf("<Server> : %s\n",buff);
    while (1) {
        char cmd[1024] = {0};
        int flag = 0;
        scanf("%[^\n]s",cmd);
        getchar();
        if (!strlen(cmd)) continue;
        //s a.txt
        //r
        if (strlen(cmd) == 1 && cmd[0] == 'r') {
            flag |= RECV;
            send(sockfd, (char *)&flag, sizeof(int), 0);
            printf("Here Recv!\n");
        } else if (cmd[0] == 's' && cmd[1] == ' ') {
            flag |= SEND;
            char file[512] = {0}, name[512] = {0}, buff[1024] = {0};
            int ack = 0;
            strcpy(file, cmd + 2);
            send(sockfd, (char *)&flag, sizeof(int), 0);
            printf("filename = %s\n",file);
            FILE *fp = fopen(file, "r");
            fseek(fp,0, SEEK_END);
            ssize_t size = ftell(fp);
            send(sockfd, (void *)&size, sizeof(size), 0);
            get_name(file, name);
      ??w_?      send(sockfd, (void *)&name, strlen(name), 0);
            recv(sockfd, (char *)&ack, sizeof(int), 0);
            if (!ack) {
                printf("Error in Ack!\n");
                exit(1);
            }
            fseek(fp, 0, SEEK_SET);
            sleep(5);
            while(1) { 
                int rsize = fread(buff, 1, 1024, fp);
                if (rsize <= 0) {
                    printf("read success!...\n");
                    break;
                }
                send(sockfd, buff, strlen(buff), 0);
            }
            fclose(fp);
        }else {
            flag |= NORMAL;
            send(sockfd, (char *)&flag, sizeof(int), 0);
            send(sockfd, cmd, strlen(cmd),0);
        }

    }


}
har *)&flag, sizeof(int), 0);
            printf("filename = %s\n",file);
            FILE *fp = fopen(file, "r");
            fseek(fp,0, SEEK_END);
            ssize_t size = ftell(fp);
            send(sockfd, (void *)&size, sizeof(size), 0);
            get_name(file, name);
      ??w_?
