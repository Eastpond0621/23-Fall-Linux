#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>  // 추가: fcntl 헤더 파일을 포함
#include <time.h>

#define MAXLINE 256
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_GREEN   "\x1b[32m"

int fd1, fd2;
char* textColor = ANSI_COLOR_RESET;

void *get() {
    char inmsg[MAXLINE + 30];
    int n;
    int len;
    while (1) {
        n = read(fd1, inmsg, MAXLINE);
        len = strlen(inmsg);
        inmsg[len - 1] = '\0';
        printf("%s%s%s\n", textColor, inmsg, ANSI_COLOR_RESET);
    }
}

void *send() {
    char inmsg[MAXLINE];
    time_t rawtime;
    struct tm *timeinfo;
    char time_str[20];
    char sendInfo[MAXLINE + 30];
    while (1) {
        fgets(inmsg, MAXLINE, stdin);
        if (strcmp(inmsg, "!red\n") == 0) {
            textColor = ANSI_COLOR_RED;
        } else if (strcmp(inmsg, "!green\n") == 0) {
            textColor = ANSI_COLOR_GREEN;
        } else if (strcmp(inmsg, "!blue\n") == 0) {
            textColor = ANSI_COLOR_BLUE;
        } else if (strcmp(inmsg, "!reset\n") == 0) {
            textColor = ANSI_COLOR_RESET;
        } else {
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);
            snprintf(sendInfo, sizeof(sendInfo), "[%s]Client2: %s", time_str, inmsg);
            write(fd2, sendInfo, strlen(sendInfo) + 1);
        }
    }
}

int main() {
    int n;
    pthread_t t1, t2;

    fd1 = open("./chatfifo3", O_RDONLY);
    fd2 = open("./chatfifo4", O_WRONLY);
    if (fd1 == -1 || fd2 == -1) {
        perror("open");
        exit(1);
    }

    printf("* Client starts\n");
    pthread_create(&t1, NULL, get, NULL);
    pthread_create(&t2, NULL, send, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

