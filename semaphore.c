#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>

int signal=0;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void *t_wait();
void *t_signal();

int main(){
    pthread_t t1;
    pthread_t t2;
    int status1, status2;
    status1=pthread_create(&t1,NULL,t_wait,NULL);
    status2=pthread_create(&t2,NULL,t_signal,NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    return 0;
}

void *t_wait(){
    pthread_mutex_lock(&mutex);

    if(signal==0){
        printf("It is not my turn. Let me sleep\n");
        pthread_cond_wait(&cond,&mutex);
        printf("I got signal to do the work\n");
    }
    pthread_mutex_unlock(&mutex);
}

void *t_signal(){
    pthread_mutex_lock(&mutex);
    printf("Resource locked\n");
    signal=1;
    if(signal==1){
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);
}