#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAXLINE 256

int fd1, fd2, fd3, fd4;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 클라이언트의 사용자 이름 저장 배열
char user_names[2][MAXLINE];

void setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

// ReadC1AndSendC2 함수 내부 수정
void *ReadC1AndSendC2() {
    int n;
    char msg[MAXLINE];
    char user_msg[MAXLINE + 30];
    while (1) {
        n = read(fd2, msg, MAXLINE);
        if (n == -1) {
            break;
        } else {
            pthread_mutex_lock(&mutex);
            // 크기를 충분히 크게 설정하여 경고를 방지
            snprintf(user_msg, sizeof(user_msg), "%s: %s", user_names[0], msg);
            pthread_mutex_unlock(&mutex);

            write(fd3, user_msg, strlen(user_msg) + 1);
        }
    }
}

// ReadC2AndSendC1 함수 내부 수정
void *ReadC2AndSendC1() {
    int n;
    char msg[MAXLINE];
    char user_msg[MAXLINE + 30];
    while (1) {
        n = read(fd4, msg, MAXLINE);
        if (n == -1) {
            break;
        } else {
            pthread_mutex_lock(&mutex);
            // 크기를 충분히 크게 설정하여 경고를 방지
            snprintf(user_msg, sizeof(user_msg), "%s: %s", user_names[1], msg);
            pthread_mutex_unlock(&mutex);

            write(fd1, user_msg, strlen(user_msg) + 1);
        }
    }
}

int main() {
    int n;
    char msg[MAXLINE];
    pthread_t t1, t2;
    
    if (mkfifo("./chatfifo1", 0666) == -1) {
        perror("mkfifo");
        exit(1);
    }

    if (mkfifo("./chatfifo2", 0666) == -1) {
        perror("mkfifo");
        exit(2);
    }
    if (mkfifo("./chatfifo3", 0666) == -1) {
        perror("mkfifo");
        exit(1);
    }

    if (mkfifo("./chatfifo4", 0666) == -1) {
        perror("mkfifo");
        exit(2);
    }

    fd1 = open("./chatfifo1", O_WRONLY);
    fd2 = open("./chatfifo2", O_RDONLY);
    fd3 = open("./chatfifo3", O_WRONLY);
    fd4 = open("./chatfifo4", O_RDONLY);

    if (fd1 == -1 || fd2 == -1 || fd3 == -1 || fd4 == -1) {
        perror("open");
        exit(3);
    }

    printf("서버 시작\n");

    // 각 클라이언트의 사용자 이름을 입력 받음
    printf("Enter a username for Client1: ");
    fgets(user_names[0], MAXLINE, stdin);
    user_names[0][strlen(user_names[0]) - 1] = '\0'; // 개행 문자 제거

    printf("Enter a username for Client2: ");
    fgets(user_names[1], MAXLINE, stdin);
    user_names[1][strlen(user_names[1]) - 1] = '\0'; // 개행 문자 제거

    pthread_create(&t1, NULL, ReadC1AndSendC2, NULL);
    pthread_create(&t2, NULL, ReadC2AndSendC1, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

