/*************************************************************************
	> File Name: 1.sum.c
	> Author: longkejie
	> Mail:1721248012@qq.com 
	> Created Time: Thu 14 Jan 2021 06:12:57 PM CST
 ************************************************************************/
#include "head.h"
char num_file[] = "./num";

char lock[] = "./lock";

struct Msg {
    int now;
    int sum;
};

struct Msg num;

size_t set_num(struct Msg *msg) {
    FILE *f = fopen(num_file,"w");
    if (f == NULL) {
        perror("fopen");
        return -1;
    }
    size_t nwrite = fwrite(msg, 1, sizeof(struct Msg), f);
    fclose(f);
    return nwrite;
}

size_t get_num(struct Msg *msg) {
    FILE *f = fopen(num_file,"r");
    if (f == NULL) {
        perror("fopen");
        return -1;
    }
    size_t nread = fread(msg, 1, sizeof(struct Msg), f);
    fclose(f);
    return nread;
}

void do_add(int end, int pid_num) {
    while (1) {
        FILE *f = fopen(num_file, "r");
        flock(f->_fileno,LOCK_EX);
        fread(&num, sizeof(struct Msg), 1, f);
        if (num.now > end + 1) {
            fclose(f);
            break;
        }
        printf("The <%d>th Chile : now = %d, sum = %d\n",pid_num, num.now, num.sum);
        num.sum += num.now;
        num.now++;
        set_num(&num);
        flock(f->_fileno, LOCK_UN);
    }
} 

int main (int argc, char **argv) {
    int opt;
    int start = 0 ;
    int end = 0;
    int ins = 0;
    while ((opt = getopt(argc, argv,"s:e:i:")) != -1) {
        switch(opt) {
            case 's':
                start = atoi(optarg);
                break;
            case 'e':
                end = atoi(optarg);
                break;
            case 'i':
                ins = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage : %s -s start_num -e end_num\n",argv[0]);
                exit(1);
        }
    }
    printf("start = %d end = %d\n", start, end);
    num.now = start;
    num.sum = 0;
    set_num(&num);
    int x = 0;
    pid_t pid;
    for (int i = 1; i <= ins; ++i) {
        if ((pid = fork()) < 0) {
            perror("fork()");
            return -1;
        }
        if (pid == 0) {
            x = i;
            break;
        }
    }
     
    if (pid > 0) {
        for (int i = 1; i <= ins; ++i) {
            wait(NULL);
        }
    }else if (pid == 0) {
        do_add(end,x);
        exit(1);
    }
    get_num(&num);
    return 0;


}

