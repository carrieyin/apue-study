#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>

using namespace std;

#define MAXLINE 4096

static void sig_pipe(int);

int main()
{
    int n, fd1[2], fd2[2];
    pid_t pid;
    char line[MAXLINE];

    if(signal(SIGPIPE, sig_pipe) == SIG_ERR)
    {
        perror("error signal");
    }

    if(pipe(fd1) < 0 || pipe(fd2) < 0)
    {
        perror("error pipe");
    }

    if((pid = fork()) < 0)
    {
        perror("error fork");
    }
    else if(pid > 0) /* parent*/
    {
        close(fd1[0]);
        close(fd2[1]);

        while ((n = read(STDIN_FILENO, line, MAXLINE)) > 0)
        {
            line[n] = 0;
            
            if(write(fd1[1], line, n) != n)
            {
                perror("error write");
            }
        
            if(read(fd2[0], line, MAXLINE) < 0)
            {
                perror("error read");
            }
            //printf("result : %s\n", string(line).c_str());
            if(fputs(line, stdout) == EOF)
            {
                perror("error fputs");
            }
        }
    }
    else /* child */
    {
        close(fd1[1]);
        close(fd2[0]);

        if(fd1[0] != STDOUT_FILENO)
        {
            if(dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO)
            {
                perror("error dup stdin");
            }

            close(fd1[0]);
        }

        if(fd2[1] != STDOUT_FILENO)
        {
            if(dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO)
            {
                perror("error dup stdout");
            }

            close(fd2[1]);
        }

        if(execl("./add2", "add2", (char*)0) < 0)
        {
            perror("error add");
        }
        
        exit(0);
    }
    
}

static void sig_pipe(int signo)
{
    printf("SIGPIPE caught \n");
    exit(1);
}
