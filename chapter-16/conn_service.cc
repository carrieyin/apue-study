#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <signal.h>

#define BUFLEN    128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

void err_quit(char * str)
{
    printf("%s \n", str);
    exit(0);
}

void err_sys(char* str)
{
    printf("%s \n", str); 
}

int initserver(int type, const struct sockaddr * addr, socklen_t alen, int qlen)
{
    int fd;
    int err = 0;
    
    if((fd = socket(addr->sa_family, type, 0)) < 0)
    {
        err_quit("socket error");
    }

    if(bind(fd, addr, alen) < 0)
    {
        goto errout;
    }

    if(type == SOCK_STREAM || type == SOCK_SEQPACKET)
    {
        if(listen(fd, qlen) < 0)
        {
            goto errout;
        }
    }
    return fd;

    errout:
        err = errno;
        close(fd);
        errno = err;
        return -1;

}


void
serve(int sockfd)
{
    int        clfd;
    FILE    *fp;
    char    buf[BUFLEN];

    //set_cloexec(sockfd);
    for (;;) {
        if ((clfd = accept(sockfd, NULL, NULL)) < 0) {
            syslog(LOG_ERR, "ruptimed: accept error: %s",
              strerror(errno));
            exit(1);
        }
        //set_cloexec(clfd);
        if ((fp = popen("/usr/bin/uptime", "r")) == NULL) {
            sprintf(buf, "error: %s\n", strerror(errno));
            send(clfd, buf, strlen(buf), 0);
        } else {
            while (fgets(buf, BUFLEN, fp) != NULL)
                send(clfd, buf, strlen(buf), 0);
            pclose(fp);
        }
        close(clfd);
    }
}

void daemonize(const char* cmd)
{
    int i, fd0, fd1,fd2;
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    umask(0);
    if(getrlimit(RLIMIT_NOFILE, &rl) < 0)
    {
        err_quit("canot get file limit");
    }

    if((pid = fork()) < 0)
    {
        err_quit("canot fork");
    }
    else if(pid != 0)
    {
        exit(0);
    }

    setsid();

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    if(sigaction(SIGHUP, &sa, NULL) < 0)
    {
        err_quit("error sighup");
    }
    if((pid = fork()) < 0)
    {
        err_quit("error fork");
    }
    else if(pid > 0)
    {
        exit(0);
    }

    chdir("/");

    if(rl.rlim_max == RLIM_INFINITY)
    {
        rl.rlim_max = 1024;
    }
    for (int  i = 0; i < rl.rlim_max; i++)
    {
        close(i);
    }
    
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if(fd0 != 0 || fd1 != 1 || fd2 != 2)
    {
        syslog(LOG_ERR, "unexpected file descriptor");
        exit(1);
    }

}

int
main(int argc, char *argv[])
{
    struct addrinfo    *ailist, *aip;
    struct addrinfo    hint;
    int                sockfd, err, n;
    char            *host;

    if (argc != 1)
        err_quit("usage: ruptimed");
    if ((n = sysconf(_SC_HOST_NAME_MAX)) < 0)
        n = HOST_NAME_MAX;    /* best guess */
    if ((host = (char *)malloc(n)) == NULL)
        err_sys("malloc error");
    if (gethostname(host, n) < 0)
        err_sys("gethostname error");
    daemonize("ruptimed");
    memset(&hint, 0, sizeof(hint));
    hint.ai_flags = AI_CANONNAME;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    if ((err = getaddrinfo(host, "ruptime", &hint, &ailist)) != 0) {
        syslog(LOG_ERR, "ruptimed: getaddrinfo error: %s",
          gai_strerror(err));
        exit(1);
    }
    for (aip = ailist; aip != NULL; aip = aip->ai_next) {
        if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr,
          aip->ai_addrlen, QLEN)) >= 0) {
            serve(sockfd);
            exit(0);
        }
    }
    exit(1);
}