#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>

//the idea is taken from here https://docs.oracle.com/javase/tutorial/essential/concurrency/deadlock.html
//
// Coffman conditions
// - mutual exclusion - each thread holds a lock when printing the object's lock when printing
// - resource holding - when first thread goes to echo and gets Jonhnny's lock, it tries to lock Cash's lock in echoBack
// - no preemption - only the thread that locks Johnny's lock in echo can unlock it
// - circular wait - first thread locks Johny's mutex in echo and tries to acquire Cash's lock, at the same time the second thread locks Cash's mutex in echo and tries to acquire Johny's mutex in echoBack, so they end up waiting on each other

using namespace std;

class Person;

struct Conversation {
    Person* friend1;
    Person* friend2;
};

class Person {
    public:
        Person(string name_) {
            pthread_mutex_init(&mutexLock, NULL);
            this->name = name_;
        }

        void echo(void *friendPtr) {
            Person *fr = (Person*)friendPtr;
            pthread_mutex_lock(&mutexLock);
            cout << pthread_self() << " Person " << name << " echoes to " << fr->name << endl;
            fr->echoBack(this);
            pthread_mutex_unlock(&mutexLock);
        }

        void echoBack(Person *fr) {
            pthread_mutex_lock(&mutexLock);
            cout << pthread_self() << " Person " << name << " echoes back to " << fr->name << endl;
            pthread_mutex_unlock(&mutexLock);
        }

        static void* threadEntrance(void *conversation) {
            Conversation* c = (Conversation *)conversation;
            c->friend1->echo(c->friend2);
            pthread_exit(NULL);
        }

    private:
        string name;
        pthread_mutex_t mutexLock;
};

int main() {
    Person *johny = new Person("Johnny");
    Person *cash = new Person("Cash");

    Conversation conversation1;
    conversation1.friend1 = johny;
    conversation1.friend2 = cash;

    Conversation conversation2;
    conversation2.friend1 = cash;
    conversation2.friend2 = johny;

    pthread_t threads[2];
    pthread_create(&threads[0], NULL, Person::threadEntrance, (void*)&conversation1);
    pthread_create(&threads[1], NULL, Person::threadEntrance, (void*)&conversation2);
    pthread_exit(NULL);
}
