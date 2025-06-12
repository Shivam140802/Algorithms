#include <stdio.h>         
#include <pthread.h>        
#include <stdlib.h>         
#include <unistd.h>         
#define BUFFER_SIZE 5      

int buffer[BUFFER_SIZE];    
int count = 0;             
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_empty = PTHREAD_COND_INITIALIZER;

void *producer(void *param) {
    int item;
    while (1) {
        item = rand() % 100;  
        pthread_mutex_lock(&mutex);  
        while (count == BUFFER_SIZE) {
            printf("Buffer is full. Producer is waiting...\n");
            pthread_cond_wait(&cond_full, &mutex);  
        }
        buffer[count] = item;
        count++;
        printf("Producer produced %d (Buffer count: %d)\n", item, count);
        pthread_cond_signal(&cond_empty);
        pthread_mutex_unlock(&mutex);  
        sleep(1);  
    }
}

void *consumer(void *param) {
    int item;
    while (1) {
        pthread_mutex_lock(&mutex);  
        while (count == 0) {
            printf("Buffer is empty. Consumer is waiting...\n");
            pthread_cond_wait(&cond_empty, &mutex);  
        }
        item = buffer[count - 1];
        count--;
        printf("Consumer consumed %d (Buffer count: %d)\n", item, count);
        pthread_cond_signal(&cond_full);
        pthread_mutex_unlock(&mutex);  
        sleep(3);  
    }
}

int main() {
    pthread_t prod_thread, cons_thread;  
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    return 0;
}
