#include "Funcs.h"

int main(int argc, char *argv[]) {

    int i;
    int numProcesses;
    int shmid;
    clock_t start, end;
    double cpuTime;
    char *token;
    int *sharedArr;
    int size = 0;
    int capacity = 256;
    char buffer[BUFFER_SIZE];

    /* Malloc array of integers */
    int *arr = malloc(capacity * sizeof(int));
    if (arr == NULL) {
        perror("Failed malloc in main\n");
        exit(EXIT_FAILURE);
    }

    /* Check for arguments */
    if (argc < 2) {
        printf("Usage: %s <numProcesses>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numProcesses = atoi(argv[1]);

    if (argc == 2) { /* Read array from stdin */
        printf("Enter the array elements (space-separated): ");
        fgets(buffer, sizeof(buffer), stdin);
        token = strtok(buffer, " \n");
        while (token != NULL) {
            arr[size++] = atoi(token);
            token = strtok(NULL, " \n");
        }
    }
    else if (argc == 3) { /* Read array from pipe */
        size = atoi(argv[2]);
        if (read(STDIN_FILENO, arr, size * sizeof(int)) == -1) {
            perror("Failed to read from pipe\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Create shared memory segment */
    shmid = shmget(IPC_PRIVATE, size * sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed in Main\n");
        exit(EXIT_FAILURE);
    }

    /* Attach shared memory segment */
    sharedArr = (int*)shmat(shmid, NULL, 0);
    if (sharedArr == (int*)-1) {
        perror("shmat failed in Main\n");
        exit(EXIT_FAILURE);
    }

    /* Copy array to shared memory */
    for (i = 0; i < size; i++) {
        sharedArr[i] = arr[i];
    }

    /* Print Initial array */
    printf("Initial Array: [");
    for (i = 0; i < size; i++) {
        printf("%d", sharedArr[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    start = clock();
    sort(sharedArr, size, numProcesses);
    end = clock();
    cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC * 1000.0;

    /* Print Sorted Array */
    printf("Sorted array: [");
    for (i = 0; i < size; i++) {
        printf("%d", sharedArr[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    printf("Processes: %d\n", numProcesses);
    printf("Time to sort: %.2f ms\n", cpuTime);

    /* Detach shared memory segment */
    if (shmdt(sharedArr) == -1) {
        perror("shmdt failed in Main\n");
        exit(EXIT_FAILURE);
    }

    /* Remove shared memory segment */
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl failed in Main\n");
        exit(EXIT_FAILURE);
    }

    free(arr);

    return 0;
}