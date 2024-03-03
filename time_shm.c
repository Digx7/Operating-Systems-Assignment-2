#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <string.h>
#include <bits/mman-linux.h>

void* create_shared_memory(size_t size) {
  // Our memory buffer will be readable and writable:
  int protection = PROT_READ | PROT_WRITE;

  // The buffer will be shared (meaning other processes can access it), but
  // anonymous (meaning third-party processes cannot obtain an address for it),
  // so only this process and its children will be able to use it:
  int visibility = MAP_SHARED| MAP_ANONYMOUS;

  // The remaining parameters to `mmap()` are not important for this use case,
  // but the manpage for `mmap` explains their purpose.
  return mmap(NULL, size, protection, visibility, -1, 0);
}

int main(int argc, char** argv) {

    printf("Time_Shm\n");

    // Create shared memory location
    void* shmem = create_shared_memory(8);

    // Create child process
    int pid = fork();

    if (pid == 0) {
        // Child

        // Get Starting Time
        struct timeval startingTime;
        gettimeofday(&startingTime, 0);

        // Write value to shared memory location
        memcpy(shmem, &startingTime, sizeof(startingTime));

        // Get sub command and its arguments
        char* command = argv[1];
        char* argument_list[] = {command, argv[2], NULL};

        // Run sub command
        execvp(command, argument_list);

        exit(0);
    } else {
        // Parent
        wait(NULL);

        // Get Ending Time
        struct timeval endingTime;
        gettimeofday(&endingTime, 0);

        // Get Starting Time from shared memory
        struct timeval startingTime;
        memcpy(&startingTime, shmem, sizeof(startingTime));

        // Get Elapsed Time
        struct timeval elapsedTime;
        timersub(&endingTime, &startingTime, &elapsedTime);

        printf( "\nElapsed time: %d.%d seconds\n", elapsedTime.tv_sec, elapsedTime.tv_usec );

        exit(0);
    }
}