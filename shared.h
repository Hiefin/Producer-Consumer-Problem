#ifndef SHARED_H
#define SHARED_H

#include <semaphore.h>          // Semaphore Library

#define BUFFER_SIZE 2            // Table can only hold 2 items

struct shared_data {
    int buffer[BUFFER_SIZE];    // Shared Array for items
    int in;                     // Producer Index
    int out;                    // Consumer Index
    sem_t empty;                // Semaphore counting empty slots
    sem_t full;                 // Semaphore counting full slots
    sem_t mutex;                // Semaphore for mutual exclusion
};

#define SHM_KEY 5555               // Shared memory key

#endif
