#include <iostream>
#include <unistd.h>

using namespace std;

// mutual exclusion is guaranteed
// the problem is that the threads are always executing in the same order, which means that we have some notion of static priority, which is explicitly not allowed in the paper

#define COUNT 3

int turn = 0;

void *PrintMessages(void *threadid) {
    long tid = (long)threadid;
    while(true) {
        if (turn == tid) {
            //critical section
            cout << "Thread " << tid << " is in critical section" << endl;
            sleep(1);
            cout << "Thread " << tid << " is going out of critical section" << endl;
            //end critical section
            turn = (turn + 1) % COUNT;
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[COUNT];
    for (long i = 0; i < COUNT; i++) {
        int rc = pthread_create(&threads[i], NULL, PrintMessages, (void *)i);
        if (rc) {
            cout << "can't create thread" << endl;
            exit(-1);
        }
    }
    pthread_exit(NULL);
}
