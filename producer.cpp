// Producer
// Jacob Drahuschak jr.
// 10/26/25

#include <iostream>     // For input/output
#include <sys/ipc.h>    // Shared Memory Keys
#include <sys/shm.h>    // Shared Memory Operations like shmat, shmget
#include <unistd.h>     // for sleep()
#include "shared.h"     // Struct and Semaphore defs

int main() {
    int shmid = shmget(SHM_KEY, sizeof(shared_data), 0666 | IPC_CREAT);  // Shared Memory get using personalized key, with the size of Struct.
    shared_data* data = (shared_data*)shmat(shmid, nullptr, 0);  // Shared Memory attach so we can use shared struct like a normal struct

    // Initialize semaphores
    sem_init(&data->empty, 1, BUFFER_SIZE);  // Sets Empty Semaphore to BUFFER_SIZE because it starts empty
    sem_init(&data->full, 1, 0);             // Sets Full Semaphore to 0 because it is all empty
    sem_init(&data->mutex, 1, 1);            // Unlocks Mutex Buffer
    data->in = 0;                            // Sets Producer Index to 0
    data->out = 0;                           // Sets Consumer Index to 0

    int item = 0;                            // Initilize Counter to help better understand which item is being changed

    while (true) {
        sem_wait(&data->empty);              // Waits till there is an empty slot in buffer
        sem_wait(&data->mutex);              // Locks Buffer - enforcing mutual exclusion

        // CRITICAL SECTION

        data->buffer[data->in] = item;                      // Buffer[PRODUCER_INDEX] = item
        std::cout << "Produced: " << item << std::endl;     // Tells User what has been produced
        data->in = (data->in + 1) % BUFFER_SIZE;            // Updates Producer Index while wrapping around Buffer
        item++;                                             // Increase Item counter

        // CRITICAL SECTION END

        sem_post(&data->mutex);                              // Unlocks Buffer
        sem_post(&data->full);                               // Signals that a slot is available for the consumer

        sleep(1);                                            // Slows down program for visibility
    }

    shmdt(data);                                             // Detach process from shared memory
    return 0;
}
