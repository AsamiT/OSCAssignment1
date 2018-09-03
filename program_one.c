/*
**
**  program_one.c
**  "Program A" for Programming Assignment I
**  3 September 2018
**
**  (c) Robert Maloy, 2018; All rights reserved.
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> /* pid_t */
#include <unistd.h>  /* _exit, fork */

pid_t pid; //pre-processor declaration of a PID value.

int main() {
    int fd[2]; //create a pipeline
    int int_Array[] = {2,3,7,-1,10,6}; //declare our array
    int arraySum; //the total sum of the array
    int arraySize = sizeof(int_Array)/sizeof(int); //create an integer value to represent the length of the array.
    //printf("%d\n", arraySize); //debug: make sure our integer value above is correct.
    
    printf("I'm the parent process, my pid is %d.\n", getpid());
    pipe(fd); //this will actually declare the pipeline.
    pid = fork(); //this formally forks the application in twain.

    if (pid == -1) {
        printf("Error: I couldn't fork myself. Sorry.\n"); //bog standard catch error
        exit(0);
    }
    else if (pid == 0)
    {
        // child process
        for (int i=0; i < arraySize; i++) {
            arraySum += int_Array[i]; //total summation of the array
        }
        printf("-------------------\n"); //divider
        printf("I'm the child process, my pid is %d.\n", getpid()); //grab the actual pid
        printf("Array sum is %d, and I will now be sending it to my parent.\n", arraySum); //print result
        close(fd[0]); //close read-descriptor
        write(fd[1], &arraySum, sizeof(arraySum)); //write-descriptor pushes data into pipeline
        close(fd[1]); //close write-descriptor
    }
    else {
        //  parent process
        wait(NULL); //tell the parent to wait until the child is done.
        close(fd[1]); //close write-descriptor as we are not using it
        read(fd[0], &arraySum, sizeof(arraySum)); //read in data from the pipeline
        printf("-------------------\n"); //divider
        printf("I'm the parent process, my pid is %d.\n", getpid()); //grab pid
        printf("%d is the final array sum.\n", arraySum); //print array sum
        exit(0); //terminate program
    }


    return 0;
}
