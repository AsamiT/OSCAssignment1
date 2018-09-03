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
    int int_Array[] = {2,3,7,-1,10,6}; //declare our array
    int arraySize = sizeof(int_Array)/sizeof(int); //create an integer value to represent the length of the array.
    //printf("%d\n", arraySize); //debug: make sure our integer value above is correct.

    pid = fork();

    if (pid == -1) {
        printf("Error: I couldn't fork myself. Sorry.\n");
        exit(0);
    }
    else if (pid == 0)
    {
        printf("I'm the child process!\n");
        sleep(1);
        _exit(0);
    }
    else {
        printf("I'm the parent!\n");
        sleep(1);
        exit(0);
    }


    return 0;
}
