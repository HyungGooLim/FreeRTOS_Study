#include <stdio.h>
#include <pthread.h>
#include <unistd.h>  // for sleep function

// Task 1: Prints a message every 1 second
void* task1(void* arg) {
    while (1) {
        printf("Task 1: Executing every 1 second\n");
        sleep(1);  // 1-second delay
    }
    return NULL;
}

// Task 2: Prints a message every 2 seconds
void* task2(void* arg) {
    while (1) {
        printf("Task 2: Executing every 2 seconds\n");
        sleep(2);  // 2-second delay
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // Create Task 1
    if (pthread_create(&thread1, NULL, task1, NULL) != 0) {
        perror("Failed to create Task 1");
        return 1;
    }

    // Create Task 2
    if (pthread_create(&thread2, NULL, task2, NULL) != 0
