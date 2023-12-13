#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<mqueue.h>
int main(int argc, char* argv[]) {
	char buf[50];
	mqd_t mq;
	mq = mq_open("/mymq", O_RDWR);
	if(mq == 	-1){
		perror("mq_open()");
		exit(1);
	}
	if(mq_receive
			(mq, buf, 50, NULL) == 	-1){
		perror("mq_receive()");
		exit(2);
	}
	printf("[MQ Recv]: %s\n", buf);
	mq_close	(mq);
	return 0;
}

