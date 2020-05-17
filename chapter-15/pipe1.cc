#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

#define MAXLINE 1024
#define DEF_PAGER "/bin/more"

int main(int argc, char *argv[])
{
    int count;
    int fd[2];
    pid_t pid;
    char line[MAXLINE];
    char *pager, *argv0;
    FILE *fp;

    if((fp = fopen(argv[1], "r")) == NULL)
      perror("fopen");

    if(pipe(fd) < 0)
      perror("pipe");

    if((pid = fork()) < 0)
      perror("fork");
    else if(pid > 0) {
        close(fd[0]);
        printf("parent !!!!!\n");
        while(fgets(line, MAXLINE, fp) != NULL) {
            count = strlen(line);
            write(fd[1], line, count);
        }
        if(ferror(fp))
          perror("fgets");
        close(fd[1]);
        if(waitpid(pid, NULL, 0) < 0)
          perror("waitpid");
        exit(0);
    } else {
        close(fd[1]);
        /* 将管道复制为标准输入 */
        if(fd[0] != STDIN_FILENO) {
            if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
              perror("dup2");
            close(fd[0]);
        }
        if((pager = getenv("PAGER")) == NULL)
          pager = DEF_PAGER;
        if((argv0 = strrchr(pager, '/')) != NULL)
            argv0++;
        else
          argv0 = pager;
          printf("pager:%s\n", pager);
        if(execl(pager, argv0, NULL) < 0)
          perror("execl");
    }
    exit(0);
}