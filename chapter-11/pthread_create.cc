#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void * thr_fun1(void* arg)
{
    printf("thread 1 returning\n");
    return (void*)1;
};

void * thr_fun2(void* arg)
{
    printf("thread 1 returning\n");
    return (void*)2;
};

int main()
{
    int err;
    pthread_t tid1, tid2;
    void *tret = NULL;

    err = pthread_create(&tid1, NULL, thr_fun1, NULL);
    if(err != 0)
    {
        printf("error!\n");
        exit(0);
    }

    err = pthread_create(&tid2, NULL, thr_fun2, NULL);
    if(err != 0)
    {
        printf("error!\n");
        exit(0);
    }

    err = pthread_join(tid1, &tret);
    if(err != 0)
    {
        printf("error!\n");
        exit(0);
    }
    printf("thread 1 exit code %ld\n", (long)tret);
    err = pthread_join(tid2, &tret);
    if(err != 0)
    {
        printf("error!\n");
        exit(0);
    }
    printf("thread 2 exit code %ld\n", (long)tret);

}