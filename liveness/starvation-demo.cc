#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

#define COUNT 3

pthread_mutex_t mlock;

void *GreedyWorker(void *threadId) {
    pthread_mutex_lock(&mlock);
    long tid = (long)threadId;
    while(true) {
        cout << "Thread " << tid << " is working" << endl;
    }
    pthread_mutex_unlock(&mlock);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[COUNT];

    pthread_mutex_init(&mlock, NULL);

    for (long i = 0; i < COUNT; i++) {
        pthread_create(&threads[i], NULL, GreedyWorker, (void *)i);
    }
    pthread_exit(NULL);
}
