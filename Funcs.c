#include "Funcs.h"

void swap(int arr[], int i, int j) {
    
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
    
}

void sort(int arr[], int size, int numProcesses) {

    int i, j, k;
    pid_t pid;
    int sorted = 0;

    for (i = 0; i < size && !sorted; i++) {
        sorted = 1;

        if (i % 2 == 0) { /* Odd phase */
            for (j = 1; j < size - 1; j += 2) {
                for (k = 0; k < numProcesses; k++) {
                    pid = fork();
                    if (pid < 0) {
                        printf("Fork failed in sort\n");
                        exit(EXIT_FAILURE);
                    }
                    else if (pid == 0) { /* Child process */
                        if (arr[j] > arr[j + 1]) {
                            swap(arr, j, j + 1);
                        }
                        exit(0);
                    }
                }

                /* Wait for children here */
                for (k = 0; k < numProcesses; k++) {
                    wait(NULL);
                }
            }
        } else { /* Even Phase */
            for (j = 0; j < size - 1; j += 2) {
                for (k = 0; k < numProcesses; k++) {
                    pid = fork();
                    if (pid < 0) {
                        printf("Fork failed.\n");
                        exit(EXIT_FAILURE);
                    }
                    else if (pid == 0) { /* Child Process */
                        if (arr[j] > arr[j + 1]) {
                            swap(arr, j, j + 1);
                        }
                        exit(0);
                    }
                }

                /* Wait for all children here */
                for (k = 0; k < numProcesses; k++) {
                    wait(NULL);
                }
            }
        }

        /* Check if array is sorted */
        for (j = 0; j < size - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                sorted = 0;
                break;
            }
        }
    }

}

void randomIntegers(int arr[], int n) {

    int i;
    srand(time(NULL));
    for (i = 0; i < n; i++) {
        arr[i] = (rand() % 201) - 100;
    }

}

void passArray(int arr[], char *n, char *processes) {

    int size;
    pid_t pid;
    int fd[2];

    /* Pipe Creation */
    if (pipe(fd) == -1) {
        perror("Failed pipe creation in randomizer\n");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) {
        perror("Fork failed in passArray\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) { /* Child Process */
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        execlp("./EvenOdd", "./EvenOdd", processes, n, NULL);
        perror("Exec failed");
        exit(EXIT_FAILURE);
    }
    else { /* Parent Process */
        close(fd[0]);
        size = atoi(n);
        write(fd[1], arr, size * sizeof(int));
        close(fd[1]);
        wait(NULL);
    }

}