// Readers-writers classsic problem
// based on Problem 2 from "Concurrent Control with "Readers" and "Writers" by Courtois, Heymans and Parnas (1971)

// in this solution once the a writer wants to write it saves spot by acquiring the writerWantsToWrite, so it can execute immediately after the current readers in progress (if any) finish

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#define READS_PER_READER 5
#define WRITES_PER_WRITER 5

using namespace std;

pthread_mutex_t readCountLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t readerLockerLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t writerWantsToWrite = PTHREAD_MUTEX_INITIALIZER;
int readcount = 0;

pthread_mutex_t canWrite = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t writeCountLock = PTHREAD_MUTEX_INITIALIZER;
int writecount = 0;

void *reader(void * id) {
    int i = *(int *) id;
    for (int j = 0; j < READS_PER_READER; j++) {
        //Indentation illustrates the locks
        pthread_mutex_lock(&readerLockerLock);
            pthread_mutex_lock(&writerWantsToWrite); //if this is acquired by a writer does not allow other readers to go.
                pthread_mutex_lock(&readCountLock);
                readcount++;
                if (readcount == 1) {
                    pthread_mutex_lock(&canWrite);
                    cout << "[Reader " << i << "] There is one reader " << " and " << writecount << " writers waiting. If there are any waiting, they will get executed after" << endl;
                }
                pthread_mutex_unlock(&readCountLock);
            pthread_mutex_unlock(&writerWantsToWrite);
        pthread_mutex_unlock(&readerLockerLock);

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
        usleep(1);
        pthread_mutex_lock(&writeCountLock);
        writecount++;
        if (writecount == 1) {
            pthread_mutex_lock(&writerWantsToWrite);
            cout << "[Writer " << i << "] acquires the writerWantsToWrite so no other readers can get it until it finishes. (aka to get priority)" << endl;
        }
        pthread_mutex_unlock(&writeCountLock);

        pthread_mutex_lock(&canWrite);
        cout << "[Writer " << i << "] is writing." << endl;
        pthread_mutex_unlock(&canWrite);

        pthread_mutex_lock(&writeCountLock);
        writecount--;
        if (writecount == 0) {
            pthread_mutex_unlock(&writerWantsToWrite);
        }
        pthread_mutex_unlock(&writeCountLock);
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
