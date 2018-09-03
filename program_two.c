/*
 **
 **  program_two.c
 **  "Program B, Example I" for Programming Assignment I
 **  3 September 2018
 **
 **  (c) Robert Maloy, 2018; All rights reserved.
 **
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> /* pid_t */
#include <unistd.h>  /* _exit, fork */
#include <string.h>

int n = 2;
pid_t pids[2];

int main() {
    int fd1[2]; //declare pipeline
    int fd2[2]; //declare pipeline
    int int_Array[] = {2,3,7,-1,10,6}; //declare array
    int arraySum; //sum of the array
    int arraySize = sizeof(int_Array)/sizeof(int); //length of the array
    int divPoint = arraySize / n; //point where we divide the array.
    int sum_A;
    int sum_B;
    
    /* The following block of code is simply the allocation and memory copy of the array into two pieces so we can send them to the children processes when the time comes. */
    int *set_A = malloc(divPoint * sizeof(int));
    if (!set_A) {
        printf("Cannot allocate memory for Set A.\n");
        exit(0);
    }
    int *set_B = malloc(divPoint * sizeof(int));
    if (!set_B) {
        printf("Cannot allocate memory for Set B.\n");
        exit(0);
    }
    memcpy(set_A, int_Array, divPoint*sizeof(int));
    memcpy(set_B, int_Array+divPoint, divPoint*sizeof(int));
    
    /*
     DEBUG -- this verifies that our copied arrays work correctly.
    for (int z=0; z < divPoint; z++) {
        printf("%d\n", set_A[z]);
        printf("%d\n", set_B[z]);
    }
    */

    printf("Parent process is currently running. pid:%d\n", getpid());
    
    close(fd1[0]);
    write(fd1[1], &set_A, sizeof(set_A));
    close(fd2[0]);
    write(fd2[1], &set_B, sizeof(set_B));
    
    for (int i=0; i < n; ++i) {
        if ((pids[i] = fork()) < 0) {
            //error
            abort();
        }
        else if (pids[i] == 0) {
            if (i == 0) {
                read(fd1[0], &set_A, sizeof(set_A));
                printf("This is set A. pid:%d\n", getpid());
                sum_A = 0;
                for (int z=0; z < divPoint; z++) {
                    sum_A += set_A[z];
                }
                printf("partial sum (A) = %d\n", sum_A);
                write(fd1[1],&sum_A,sizeof(sum_A));
            }
            else if (i == 1) {
                read(fd2[0], &set_B, sizeof(set_B));
                printf("This is set B. pid:%d\n", getpid());
                sum_B = 0;
                for (int z=0; z < divPoint; z++) {
                    sum_B += set_B[z];
                }
                printf("partial sum (B) = %d\n", sum_B);
                write(fd2[1], &sum_B, sizeof(sum_B));
            }
            exit(0);
        }
    }
    
    while (n > 0) {
        wait(NULL);
        --n;
    }
    printf("Now you're back to the parent process. pid:%d\n", getpid());
    arraySum = sum_B;
    printf("total sum (A+B) = %d\n", arraySum);
}
