#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define PAGER "${PAGER:-more}"
#define MAXLINE 100

int main(int argc, char* argv[])
{
    int n;
    int fd[2];
    pid_t pid;
    char *pager, *argv0;
    char line[MAXLINE];

    FILE *fpin, *fpout;

    if(argc != 2)
    {
        printf("usage: a.out <pathname> \n");
        
    }
    fpin = fopen(argv[1], "r");
    if(fpin == NULL)
    {
        printf("canot open file \n");
        return 0;
    }
    fpout = popen(PAGER , "w");
    while(fgets(line, MAXLINE, fpin) != NULL)
    {
        n = strlen(line);
        //printf("write buf :%d\n", n);
        if(fputs(line, fpout) == EOF)
        {
            printf("write error \n");
            exit(0);
        }
    }

    if(pclose(fpout) == -1) 
    {
        printf("error pclose\n");
    }    
    
}
