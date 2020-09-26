#include <iostream>
#include <unistd.h>

using namespace std;
// mutual exclusion property is proved
// it's not starvation free, because if thread dies in a CS, then no other thread will be able to get in

#define COUNT 3

bool certainlyOutsideCS[COUNT]; //Thread i is certainly outside of the Critical Section (if false - it may be in or almost in)
bool inactive[COUNT]; //Thread i is inactive when it wants to try to enter CS
int turn = 1;

void *PrintMessages(void *threadid) {
    long tid = (long)threadid;
    while(true) {
        inactive[tid] = false;
        L1: if (turn != tid) {
            certainlyOutsideCS[tid] = true;
            if (inactive[turn]) {
                turn = tid;
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
        inactive[tid] = true;
    }
    pthread_exit(NULL);
}

int main() {
    for (int i = 0; i < COUNT; i++) {
        certainlyOutsideCS[i] = true;
        inactive[i] = true;
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
