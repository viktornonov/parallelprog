#include <iostream>
#include <unistd.h>

//Not sure if it's 100% correct because int array in C++ is not thread-safe

using namespace std;

#define COUNT 4

int level[COUNT];
int victim[COUNT];

bool otherThreadsOnSameOrHigherLevel(int tid, int currentLevel) {
    for (int k = 0; k < COUNT; k++) {
        if (k != tid && level[k] >= currentLevel) {
            return true;
        }
    }
    return false;
}

void *PrintMessages(void *threadid) {
    long tid = (long)threadid;
    while(true) {
        for (int currentLevel = 1; currentLevel < COUNT; currentLevel++) {
            level[tid] = currentLevel;
            victim[currentLevel] = tid;
            while (otherThreadsOnSameOrHigherLevel(tid, currentLevel) && victim[currentLevel] == tid) {
                ;//busy wait
            }
        }

        //critical section
        cout << "Thread " << tid << " is in critical section" << endl;
        //sleep(1);
        cout << "Thread " << tid << " is going out of critical section" << endl;
        //end critical section

        level[tid] = 0; //send the thread back to the beginning
    }
    pthread_exit(NULL);
}

int main() {
    for (int i = 0; i < COUNT; i++) {
        level[i] = 0;
        victim[i] = 0;
    }
    pthread_t threads[COUNT];
    for (long i = 0; i < COUNT; i++) {
        long rc = pthread_create(&threads[i], NULL, PrintMessages, (void *)i);
        if (rc) {
            cout << "can't create thread" << endl;
            exit(-1);
        }
    }
    pthread_exit(NULL);
}
