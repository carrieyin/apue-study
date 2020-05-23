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
    //1.打开消息队列   msgget(key_t key , int msgflg);
    msgid = msgget(IPC_KEY,0);
    if(msgid<0)
    {
        perror("msgget error");
        exit(-1);
    }
    
    struct msgbuff buf;   
    while(1)
    {
        //读键盘输入
        memset(&buf,0x00,sizeof(struct msgbuff));   //首先清理  
        printf("[c]>");
        fflush(stdout);
        scanf("%s",buf.mtext);
        //2.开始向消息队列中写入消息   
        buf.mtype = TYPE_C;
        msgsnd(msgid,&buf,strlen(buf.mtext),0);  //作为客户端端，想消息队列中插入的消息类型为TYPE_C
        //3.冲消息队列中读出来自服务器的消息
        memset(&buf,0x00,sizeof(struct msgbuff));  
        msgrcv(msgid,&buf,sizeof(buf.mtext),TYPE_S,0);  //作为客户端，希望读出来自服务端的消息 类型为TYPE_S
        //打印消息到终端
        printf("[S]:%s\n",buf.mtext);
    }
    //4.删除消息队列
    msgctl(msgid,IPC_RMID,NULL);
 
    return 0;
}