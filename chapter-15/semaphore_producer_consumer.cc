#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

#define MAX 100
#define BUFSIZE 10
int buf[BUFSIZE] = {0};
    int in = 0;
    int out = 0;
    sem_t full, empty;
void* producer(void* arg)
{
    int i;
    for(i = 0; i <= MAX; i++)
    {
        sem_wait(&full);
        buf[in++] = i;
        in %= BUFSIZE;
        printf("in %d: \n", buf[in]);
        sem_post(&empty);
    }

    pthread_exit((void*)"thread1 exit\n");
}

void* consumer(void * arg)
{
    int i; 
    for(i = 1; i <= MAX; ++i)
    {
        sem_wait(&empty);
        out++;
        out %=BUFSIZE;
        printf("out %d: \n", buf[out]);
        sem_post(&full);
    }

     pthread_exit((void*)"thread2 exit\n");
}

int main()
{
    
    void *tret;
    sem_init(&full, 0, 10);
    sem_init(&empty, 0, 0);

    pthread_t tid_prodeucer, tid_consumer;
    pthread_create(&tid_prodeucer, NULL, producer, NULL);
    pthread_create(&tid_consumer, NULL, consumer, NULL);
    
    pthread_join(tid_prodeucer, &tret);
    printf("%s \n", (char*)tret);
    pthread_join(tid_consumer, &tret);
    printf("%s \n", (char*)tret);

    sem_destroy(&full);
    sem_destroy(&empty);


}
