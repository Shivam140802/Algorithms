#include <stdio.h>
#include <pthread.h>
#define array_size 50000
#define no_threads 10

int a[array_size];
int global_index = 0;
int sum = 0;
pthread_mutex_t mutex;

void* increment(void* arg) {
    int local_index, partial_sum = 0;
    do {
        pthread_mutex_lock(&mutex);
        local_index = global_index;
        global_index++;
        pthread_mutex_unlock(&mutex);

        if (local_index < array_size) {
            partial_sum += a[local_index];
        }
    } while (local_index < array_size);

    pthread_mutex_lock(&mutex);
    sum += partial_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    int i;
    pthread_t thread[no_threads];
    pthread_mutex_init(&mutex, NULL);

   
    for (i = 0; i < array_size; i++) {
        a[i] = i + 1;
    }

    
    for (i = 0; i < no_threads; i++) {
        if (pthread_create(&thread[i], NULL, increment, NULL) != 0) {
            perror("pthread create failed");
            return 1;
        }
    }

    
    for (i = 0; i < no_threads; i++) {
        if (pthread_join(thread[i], NULL) != 0) {
            perror("pthread join failed");
            return 1;
        }
    }

    printf("Sum of 1 to %d is %d\n", array_size, sum);
    printf("Global values: %d", global_index);

    pthread_mutex_destroy(&mutex);


    return 0;
}