// Readers-writers classsic problem
// based on Problem 1 from "Concurrent Control with "Readers" and "Writers" by Courtois, Heymans and Parnas (1971)

// as mentioned in the paper this solution might starve the writers

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#define READS_PER_READER 5
#define WRITES_PER_WRITER 5

using namespace std;

pthread_mutex_t readCountLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t canWrite = PTHREAD_MUTEX_INITIALIZER;
int readcount = 0;

void *reader(void * id) {
    int i = *(int *) id;
    for (int j = 0; j < READS_PER_READER; j++) {
        pthread_mutex_lock(&readCountLock);
        readcount++;
        if (readcount == 1) {
            pthread_mutex_lock(&canWrite);
            cout << "[Reader " << i << "] First reader acquires the canWrite lock to block all writers and unlocks only when there are no more Readers." << endl;
        }
        pthread_mutex_unlock(&readCountLock);

        cout << "[Reader " << i << "] is reading." << endl;
        usleep(1);

        pthread_mutex_lock(&readCountLock);
        readcount--;
        if (readcount == 0) {
            cout << "[Reader " << i << "] No more readers. Writers go ahead" << endl;
            pthread_mutex_unlock(&canWrite);
        }
        pthread_mutex_unlock(&readCountLock);
    }
}

void *writer(void *id) {
    int i = *(int *) id;
    for (int j = 0; j < WRITES_PER_WRITER; j++) {
        pthread_mutex_lock(&canWrite);

        cout << "[Writer " << i << "] is writing." << endl;
        usleep(1);

        pthread_mutex_unlock(&canWrite);
    }
}

int main() {
    pthread_t readers[5], writers[5];
    int ids[] = {0, 1, 2, 3, 4};

    for (int i = 0; i < 5; i++) {
        pthread_create(&readers[i], NULL, &reader, &ids[i]);
        pthread_create(&writers[i], NULL, &writer, &ids[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(writers[i], NULL);
    }
}
