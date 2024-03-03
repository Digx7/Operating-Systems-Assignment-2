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

int main(int argc, char** argv) {

    printf("Time_Pipe\n");

    int fd[2];
    pipe(fd);
    int pid = fork();

    if (pid == 0) {
        // Child

        // Get Starting Time
        struct timeval startingTime;
        gettimeofday(&startingTime, 0);
        // Write value to pipe
        write(fd[1], &startingTime, sizeof(startingTime));

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

        // Get Starting Time
        struct timeval startingTime;
        // Read value from pipe
        read(fd[0], &startingTime, sizeof(startingTime));

        // Get Elapsed Time
        struct timeval elapsedTime;
        timersub(&endingTime, &startingTime, &elapsedTime);

        printf( "\nElapsed time: %d.%d seconds\n", elapsedTime.tv_sec, elapsedTime.tv_usec );

        exit(0);
    }
}