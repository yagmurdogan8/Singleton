//Written by Yağmur Doğan 10963330588 '21

//!---Comment for the project is at the end of the code---!//

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#define thread_no 10

struct Singleton {
    char *data;
};


struct Singleton *singletonObjectPointer;


int addresses[thread_no];
sem_t sem;

void *runner(void *params); //the thread
struct Singleton *getInstance();

int main()
{
    int i;
    sem_init(&sem,0,1);
    pthread_t threadIds[thread_no];


    for (i = 0; i < thread_no; i++){
        pthread_create(&threadIds[i], NULL, &runner, (void *)(i));
    }

    //Waits until all threads are done
    for (i=0; i < thread_no; i++){
        pthread_join(threadIds[i], NULL);
    }

    //Controls addresses. All of them should be same
    int prevAddr=addresses[0];
    for (i=1; i < thread_no; i++){
        if(addresses[i]!=prevAddr){
            printf("Object is created more than once\n");
            return -1;
        }
        prevAddr=addresses[i];
    }
    for (i=0; i < thread_no; i++){
        printf("Singleton Addresses for each thread %x\n",addresses[i]);
    }
    printf("Successful\n");
    return 1;
}


//Threads will controlled below

void *runner(void *params)
{
    int i = (int)params;
    printf("Thread %d\n",i);
    struct Singleton *s = getInstance();
    addresses[i]=s;
    pthread_exit(0);
}//To enter to other threads

struct Singleton *getInstance(){
    if(singletonObjectPointer == NULL){ //Checks whether the object is null or not
        sem_wait(&sem); //One thread at a time
        if (singletonObjectPointer == NULL){ //Checks whether the object is null or not
            singletonObjectPointer = (struct Singleton *)malloc(sizeof(struct Singleton));
            printf("---Address of singletonObjectPointer is %x\n",singletonObjectPointer);
            singletonObjectPointer->data="This is object data";
        }
        sem_post(&sem); //Releases the semaphore for letting another thread to execute
    }
    return singletonObjectPointer;
}


/*!
                                           #-|-# Comments #-|-#

                Semaphores are another common form of synchronization that allows threads to "post"
                         and "wait" on a semaphore to control when threads wake or sleep.

         Double checked synchronization ensures that only the first few threads that try to acquire
                     the singleton have to go through the process of acquiring the lock.

                     Counting Semaphore – Its value can range over an unrestricted domain.
                    It is used to control access to a resource that has multiple instances.

!*/

