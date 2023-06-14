#ifndef FUNCS
#define FUNCS

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFFER_SIZE 256


void swap(int arr[], int i, int j);
void sort(int arr[], int size, int numProcesses);
void randomIntegers(int arr[], int n);
void passArray(int arr[], char *size, char *processes);

#endif