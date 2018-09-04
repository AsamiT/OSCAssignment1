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

void syntaxError() {
    printf("Syntax Error: one argument with integer value of 2 or 3.");
    exit(0);
}

int main(int argc, const char* argv[]) {
    
    if (!argv[1]) {
        syntaxError();
        exit(0);
    }
    
    int n = atoi(argv[1]);
    
    if (n < 2 || n > 3) {
        syntaxError();
        exit(0);
    }
   
    pid_t pids[n];

    int fd[2], fd1[2], fd2[2], fd3[2], fd4[2]; //declare pipelines
    pipe(fd);
    pipe(fd1);
    pipe(fd2);
    pipe(fd3);
    pipe(fd4);

    int int_Array[] = {2,3,7,-1,10,6}; //declare array
    int arraySum = 0; //sum of the array
    int posindex = 0; //position in the array
    int div_index = 0;
    int n_index = 0;
    int child_Sum = 0;
    int arraySize = sizeof(int_Array)/sizeof(int); //length of the array
    int divPoint = (arraySize / n); //point where we divide the array.
    int s_Array[n][divPoint];
    int unionArray[n];

    printf("Parent process is currently running, and has an array size of %d. pid:%d\n", arraySize, getpid());
    printf("Splitting array into %d pieces...\n", n);
    
    for (int a=0; a < arraySize; a++) {
        if ((a % divPoint) == 0 && (a != 0)) {
            //printf("%d | %d\n", a, divPoint);
            n_index++;
            div_index = 0;
        }
        //debug: printf("di: %d\n", div_index);
        s_Array[n_index][div_index] = int_Array[a];
        //debug: printf("sArray[%d][%d] = %d\n", n_index, div_index, s_Array[n_index][div_index]);
        div_index++;
    }
    
    write(fd[1], &child_Sum, sizeof(child_Sum));
    write(fd1[1], &s_Array, sizeof(s_Array));
    write(fd2[1], &unionArray, sizeof(unionArray));
    write(fd3[1], &divPoint, sizeof(divPoint));
    
    for (int i=0; i < n; ++i) {
        if ((pids[i] = fork()) < 0) {
            //error
            abort();
        }
        else if (pids[i] == 0) {
            for (int b=0; b < divPoint; b++) {
                //printf("%d-%d --> %d\n", i, b, s_Array[i][b]);
                child_Sum += s_Array[i][b];
            }
            //printf("%d\n", child_Sum);
            printf("Child process executed; pid=%d; child_Sum=%d\n", getpid(), child_Sum);
            write(fd4[1], &child_Sum, sizeof(child_Sum));
            exit(0);
        }
    }
    
    while (n > 0) {
        wait(NULL);
        read(fd4[0], &child_Sum, sizeof(child_Sum));
        arraySum += child_Sum;
        --n;
    }
    printf("Now you're back to the parent process. pid:%d\n", getpid());
    printf("total sum = %d\n", arraySum);
 
 }
