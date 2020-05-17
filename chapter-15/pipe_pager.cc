#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define DEF_PAGER "/bin/more"
#define MAXLINE 100

int main(int argc, char* argv[])
{
    int n;
    int fd[2];
    pid_t pid;
    char *pager, *argv0;
    char line[MAXLINE];

    FILE *fp;

    //printf("argc:%d argv[0]: %s---  argv[1]---%s\n", argc, argv[0],argv[1]);
    if(argc != 2)
    {
        printf("usage: a.out <pathname> \n");
        
    }

    if((fp = fopen(argv[1], "r")) == NULL)
    {
        printf("can not open file: %s\n", argv[1]);
    }

    if(pipe(fd) < 0)
    {
        printf("pipe error \n");
    }

    if((pid = fork()) < 0)
    {
        printf("fork error \n");
        //exit(0);
    }else if(pid > 0) /***parent***/
    {
        close(fd[0]);

        while(fgets(line, MAXLINE, fp) != NULL)
        {
            n = strlen(line);
            //printf("write buf :%d\n", n);
            if(write(fd[1], line, n) != n)
            {
                
                printf("write error \n");
                exit(0);
            }
        }
        if(ferror(fp))
        {
            printf("file error \n");
            exit(0);
        }

        close(fd[1]);

        if(waitpid(pid, NULL, 0)< 0)
        {
            printf("waitpid error \n");
            exit(0);
        }
        //exit(0);
    }
    else
    {
        /* code */
        //printf("child !!!!\n");
        close(fd[1]);

        if(fd[0] != STDIN_FILENO)
        {
            if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
            {
                printf("dup2 error to stdion\n");
                close(fd[0]);
            }
        }

        if((pager = getenv("PAGER")) == NULL)
        {
            pager = DEF_PAGER;
        }
        //printf("pager: %s\n", pager);
        if((argv0 = strrchr(pager, '/')) != NULL)
        {
            argv0++;
        }
        else
        {
            argv0 = pager;
        }
        //下面为测试管道数据是否已经写入，方便排查问题，可以删除
        /*char buf[MAXLINE];
        int n = 0;
        while((n = read(fd[0], buf, MAXLINE)) != 0)
        {
            printf("%s------", buf);
        }*/
        //printf("------buf-----%s:\n", buf);
        //！！！！！！注意此处使用execl会导致中找不到less程序，报错no such file
        if(execlp(pager, argv0, (char*)0) < 0)
        {
            char * msg;
            msg = strerror(errno);
            //printf("execl %s error, msg: %s\n", pager, msg);
            perror("execl");
            
        }
        
        exit(0);
    }
    
}
