#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAXLINE 4096
int main()
{
    int n;
    int fd[2];
    pid_t pid;
    char line[MAXLINE];

    if(pipe(fd) < 0)
    {
        printf("error \n");
    }

    if((pid = fork()) < 0)
    {
        printf("fork error \n");
    }else if(pid > 0) /***parent**/
    {
        close(fd[0]);
        write(fd[1], "HELLO\n",6);
    }else
    {
        /* code */
        close(fd[1]);
        n = read(fd[0], line, MAXLINE);
        write(STDOUT_FILENO, line, n);
    }
    

    exit(0);
}