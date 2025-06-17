#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

pthread_mutex_t rw_mutex;    
pthread_mutex_t r_mutex; 
int read_count = 0; 
int shared_data = 0; 

void* reader(void* arg) {
    int reader_id = *(int*)arg;
    pthread_mutex_lock(&r_mutex);
    read_count++;
    if (read_count == 1) { 
        pthread_mutex_lock(&rw_mutex);
    }
    pthread_mutex_unlock(&r_mutex);
    printf("Reader %d: reads shared_data = %d\n", reader_id, shared_data);
    sleep(1); 
    pthread_mutex_lock(&r_mutex);
    read_count--;
    if (read_count == 0) { 
        pthread_mutex_unlock(&rw_mutex);
    }
    pthread_mutex_unlock(&r_mutex);

    return NULL;
}

void* writer(void* arg) {
    int writer_id = *(int*)arg;
    pthread_mutex_lock(&rw_mutex);
    shared_data++; 
    printf("Writer %d: updated shared_data to %d\n", writer_id, shared_data);
    sleep(1); 
    pthread_mutex_unlock(&rw_mutex);

    return NULL;
}

int main() {
    pthread_t r_threads[5], w_threads[5];
    int ids[5];

    pthread_mutex_init(&rw_mutex, NULL);  
    pthread_mutex_init(&r_mutex, NULL); 
    for (int i = 0; i < 5; i++) {
        ids[i] = i + 1;
        pthread_create(&r_threads[i], NULL, reader, &ids[i]);
        pthread_create(&w_threads[i], NULL, writer, &ids[i]);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(r_threads[i], NULL);
        pthread_join(w_threads[i], NULL);
    }
    pthread_mutex_destroy(&r_mutex);
    pthread_mutex_destroy(&rw_mutex);

    return 0;
}
