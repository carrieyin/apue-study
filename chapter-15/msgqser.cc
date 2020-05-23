#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<errno.h>
 
//指定消息队列的KEY
#define IPC_KEY 0x12345678
 
//指定两种消息的类型，分别标识来自server和client
#define TYPE_S 1
#define TYPE_C 2
 
//定义消息的结构(已经在msg.h进行了定义)
struct msgbuff{
    long mtype;  //消息类型
    char mtext[512];  //消息的实体
};
 
int main()
{
    int msgid = -1;
    //1.创建消息队列   msgget(key_t key , int msgflg);
    msgid = msgget(IPC_KEY,IPC_CREAT|0777);
    if(msgid<0)
    {
        perror("msgget error");
        exit(-1);
    }
    
    struct msgbuff buf;   
    while(1)
    {
        //2.开始从消息队列中读出消息
        memset(&buf,0x00,sizeof(struct msgbuff));   //首先清理
        msgrcv(msgid,&buf,sizeof(buf.mtext),TYPE_C,0);  //作为服务端，指向读出来自客户端的消息TYPE_C
        //打印读到的消息
        printf("[c]:%s\n",buf.mtext);
        //读取键盘输入
        memset(&buf,0x00,sizeof(msgbuff));
        buf.mtype=TYPE_S;
        printf("[s]>");
        fflush(stdout);
        scanf("%s",buf.mtext);
        //3.往消息队列插入消息
        msgsnd(msgid,&buf,strlen(buf.mtext),0);   
    }
    //4.删除消息队列
    msgctl(msgid,IPC_RMID,NULL);
 
    return 0;
}