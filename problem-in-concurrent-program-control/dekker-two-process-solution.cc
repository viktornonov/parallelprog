#include <iostream>
#include <unistd.h>

using namespace std;

bool wantsToEnter[] = { false, false };
int turn = 1;

void *PrintMessages(void *threadid) {
    long tid = (long)threadid;
    long other_tid = (tid == 1) ? 0 : 1;;
    while(true) {
        wantsToEnter[tid] = true;
        while (wantsToEnter[other_tid]) {
            if (turn == other_tid) {
                wantsToEnter[tid] = false;
                while (turn == other_tid) {
                    ;//busy wait
                }
                wantsToEnter[tid] = true;
            }
        }

        //critical section
        cout << "Thread " << tid << " is in critical section" << endl;
        cout << "Thread " << tid << " is going out of critical section" << endl;
        //end critical section

        turn = other_tid;
        wantsToEnter[tid] = false;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[2];
    for (long i = 0; i < 2; i++) {
        int rc = pthread_create(&threads[i], NULL, PrintMessages, (void *)i);
        if (rc) {
            cout << "can't create thread" << endl;
            exit(-1);
        }
    }
    pthread_exit(NULL);
}
