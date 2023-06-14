#include "Funcs.h"

int main(int argc, char *argv[]) {

    char *n;
    char *processes = NULL;
    int *arr = NULL;

    /* Check for arguments */
    if (argc != 3) {
        printf("Usage: %s <n> <numProcesses>", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    n = argv[1];
    processes = argv[2];

    /* Malloc array for use */
    arr = malloc(atoi(n) * sizeof(int));
    if (arr == NULL) {
        perror("Failed malloc in randomizer\n");
        exit(EXIT_FAILURE);
    }

    randomIntegers(arr, atoi(n));
    passArray(arr, n, processes);

    free(arr);

    return 0;

}
