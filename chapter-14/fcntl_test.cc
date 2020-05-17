
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void pflock(struct flock *fl)
{
    printf("fl.l_type:%d,fl.l_whence::%d,fl.l_start:%d,fl.l_len:%d,fl.l_pid:%lu\n",fl->l_type,fl->l_whence,fl->l_start,fl->l_len,(long)fl->l_pid);
}
int main(void)
{
    int fd, ret;
    pid_t pid;
    struct flock fl;

    fd = open("./1", O_RDWR);
    if ( fd < 0 )
    {
        perror("open");
        return -1;
    }

    pid = fork();
    if ( pid < 0 )
    {
        perror("fork");
        close(fd);
        return -1;
    }
    else if ( pid == 0 )
    {
        printf("child pid:%lu\n",(long)getpid());
        fl.l_type = F_WRLCK;
        fl.l_whence = SEEK_SET;
        fl.l_start = 1;
        fl.l_len = 2;
        fl.l_pid = getpid();

        ret = fcntl(fd, F_GETLK, &fl);
        if ( fl.l_type != F_UNLCK )
        {
            printf("already have a file lock\n");
            pflock(&fl);
            close(fd);
            exit(0);
        }
        fl.l_type = F_WRLCK;
        printf("-----------child %d \n", fl.l_type);
        
        ret = fcntl(fd, F_SETLK, &fl);
        
        if ( ret < 0 )
        {
            perror("fcntl");
            close(fd);
            exit(0);
        }
        printf("child set flock\n");
        while(1);
    }
    else
    {
        printf("parent pid:%lu\n",(long)getpid());
        sleep(1);   //let child process get a flock before parents

        fl.l_type = F_WRLCK;
        fl.l_whence = SEEK_SET;
        fl.l_start = 1;
        fl.l_len = 2;
        fl.l_pid = getpid();

        ret = fcntl(fd, F_GETLK, &fl);
        if ( fl.l_type != F_UNLCK )
        {
            printf("already have a file lock\n");
            pflock(&fl);
            close(fd);
            exit(0);
        }
        printf("-----------parent %d \n", fl.l_type);
       
        fl.l_type = F_WRLCK;
        ret = fcntl(fd, F_SETLK, &fl);
        if ( ret < 0 )
        {
            perror("fcntl");
            close(fd);
            exit(0);
        }
         printf("parent set flock\n");
        while(1);
    }

    close(fd);
    exit(0);
}
