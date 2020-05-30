#include <poll.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

using namespace std;
 
#define NQ  3 /* number of queues */
#define MAXMSZ	512		/* maximum lenth */
#define KEY	0x123	/* key for first message queue */
 
struct threadinfo {
	int qid;// the quene id 
	int fd;// the fd used to write
};
 
struct mymesg {
	long mtype;
	char mtext[MAXMSZ];
};
 
void * helper(void *arg){
	int n;
	struct mymesg m;
	struct threadinfo *tip = (struct threadinfo *)arg;
	for(;;) {
		memset(&m, 0, sizeof(m));
		if ((n = msgrcv(tip->qid, &m, MAXMSZ, 0, MSG_NOERROR)) < 0)printf("msgrcv error\n");
		if (write(tip->fd, m.mtext, n) < 0)printf("write error\n");
	}
}
 
int main(){
	int i,n,err;
	int fd[2];
	int qid[NQ];
	struct pollfd pfd[NQ];
	struct threadinfo ti[NQ];
	pthread_t tid[NQ];
	char buf[MAXMSZ];
	for (i = 0; i < NQ; i++) {
		if ((qid[i] = msgget((KEY+i), IPC_CREAT|0666)) < 0)printf("msgget error\n");
		cout<<"the msgque id of server is "<<qid[i]<<endl;
		printf("queue ID %d is %d\n", i, qid[i]);
		if (socketpair(AF_UNIX, SOCK_DGRAM, 0, fd) < 0)printf("socketpair error\n");
			cout<<"fd[0] is: "<<fd[0]<<" fd[1] is: "<<fd[1]<<endl;
		pfd[i].fd = fd[0];
		pfd[i].events = POLLIN;
		ti[i].qid = qid[i];
		ti[i].fd = fd[1];
		if ((err = pthread_create(&tid[i], NULL, helper, &ti[i])) != 0)printf("pthread_create error\n");
	}
	for (;;) {//reading data
		if (poll(pfd, NQ, -1) < 0)printf("poll error\n");
		for (i = 0; i < NQ; i++) {
			if (pfd[i].revents & POLLIN) {
				if ((n = read(pfd[i].fd, buf, sizeof(buf))) < 0)printf("read error\n");
				buf[n] = 0;
				printf("queue id %d, message %s\n", qid[i], buf);
			}
		}
	}
	exit(0);
}