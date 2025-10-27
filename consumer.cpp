// Consumer
// Jacob Drahuschak jr.
// 10/26/25

#include <iostream>     // For input/output
#include <sys/ipc.h>    // Shared Memory Keys
#include <sys/shm.h>    // Shared Memory Operations like shmat, shmget
#include <unistd.h>     // for sleep()
#include "shared.h"     // Struct and Semaphore defs

int main() {
    int shmid = shmget(SHM_KEY, sizeof(shared_data), 0666);      // Shared Memory get using personalized key, with the size of Struct.
    shared_data* data = (shared_data*)shmat(shmid, nullptr, 0);  // Shared Memory attach so we can use shared struct like a normal struct

    sleep(1);                                                    // Give time for producer to produce
    while (true) {
        sem_wait(&data->full);                  // Waits till there is a item in buffer
        sem_wait(&data->mutex);                 // Locks Mutex for mutual exlcusion

        int item = data->buffer[data->out];     // Reads the item at the Consumer Index
        std::cout << "Consumed: " << item << std::endl;     // Prints to console
        data->out = (data->out + 1) % BUFFER_SIZE;          // Change Consumer Index with wrapping modulus in mind

        sem_post(&data->mutex);                 // Unlocks Mutex
        sem_post(&data->empty);                 // Signals that there is a free slot of the producer

        sleep(1);                               // Needed to stop increadibly fast execution speed
    }

    shmdt(data);                                 // Detach Consumer from shared memory
    return 0;
}
