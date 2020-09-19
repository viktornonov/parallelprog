#include <iostream>
#include <unistd.h>

using namespace std;
// mutual exclusion property is proved
// it's not starvation free, because if thread dies in a CS, then no other thread will be able to get in

#define COUNT 3

bool certainlyOutsideCS[COUNT]; //Thread i is definitely outside of the Critical Section
bool waitingToCheck[COUNT]; //Thread i is waiting to check if it can enter
int threadTryingToEnterCS = 1;

void *PrintMessages(void *threadid) {
    long tid = (long)threadid;
    while(true) {
        waitingToCheck[tid] = false;
        L1: if (threadTryingToEnterCS != tid) {
            certainlyOutsideCS[tid] = true;
            if (waitingToCheck[threadTryingToEnterCS]) {
                threadTryingToEnterCS = tid;
            }
            goto L1;
        }
        else {
            certainlyOutsideCS[tid] = false;
            for (int j = 0; j < COUNT; j++) { // check if another thread didn't get into CS
                if (j != tid && !certainlyOutsideCS[j]) {
                    goto L1;
                }
            }
        }

        //critical section
        cout << "Thread " << tid << " is in critical section" << endl;
        sleep(1);
        cout << "Thread " << tid << " is going out of critical section" << endl;
        //end critical section

        certainlyOutsideCS[tid] = true;
        waitingToCheck[tid] = true;
    }
    pthread_exit(NULL);
}

int main() {
    for (int i = 0; i < COUNT; i++) {
        certainlyOutsideCS[i] = true;
        waitingToCheck[i] = true;
    }
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
