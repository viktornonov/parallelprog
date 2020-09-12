#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

bool oneWantsToAsk = false;
bool twoWantsToAsk = false;

void *correspondentOneAction(void *) {
    while (!twoWantsToAsk) {
        cout << pthread_self() << " One pauses to listen" << endl;
        sleep(1);
    }
    cout << pthread_self() << " Correspodent One wants to ask something" << endl;

    oneWantsToAsk = true;
    pthread_exit(NULL);
}

void *correspondentTwoAction(void *) {
    while (!oneWantsToAsk) {
        cout << pthread_self() << " Two pauses to listen" << endl;
        sleep(1);
    }
    cout << pthread_self() << " Correspodent One wants to ask something" << endl;
    twoWantsToAsk = true;
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[2];
    int rc = pthread_create(&threads[0], NULL, correspondentOneAction, NULL);
    if (rc) {
        cout << "can't create thread" << endl;
        exit(-1);
    }
    rc = pthread_create(&threads[1], NULL, correspondentTwoAction, NULL);
    if (rc) {
        cout << "can't create thread" << endl;
        exit(-1);
    }
    pthread_exit(NULL);
    return 0;
}
