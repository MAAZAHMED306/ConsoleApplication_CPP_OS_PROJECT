#include <stdio.h>
#include <stdbool.h>

// Constants
#define NUM_PROCESSES 5
#define NUM_RESOURCES 3

// Resources
int available[NUM_RESOURCES] = { 3, 3, 2 }; // Resources: A, B, C
int maximum[NUM_PROCESSES][NUM_RESOURCES] = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2},
    {2, 2, 2},
    {4, 3, 3}
};
int allocation[NUM_PROCESSES][NUM_RESOURCES] = {
    {0, 1, 0},
    {2, 0, 0},
    {3, 0, 2},
    {2, 1, 1},
    {0, 0, 2}
};
int need[NUM_PROCESSES][NUM_RESOURCES];

// Function to calculate and display need matrix
void calculateAndDisplayNeed() {
    printf("\nProcess\tNeed (A B C)\n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("P%d\t", i);
        for (int j = 0; j < NUM_RESOURCES; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
}

// Function to display available resources
void displayAvailableResources() {
    printf("\nAvailable Resources (A B C): ");
    for (int i = 0; i < NUM_RESOURCES; i++) {
        printf("%d ", available[i]);
    }
    printf("\n");
}

// Function to check if request can be granted
bool canGrantRequest(int process, int request[]) {
    for (int i = 0; i < NUM_RESOURCES; i++) {
        if (request[i] > need[process][i]) {
            printf("Error: Requested resource %c exceeds process P%d's need. Request denied.\n", 'A' + i, process);
            return false;
        }
        if (request[i] > available[i]) {
            printf("Error: Requested resource %c exceeds available resources. Request denied.\n", 'A' + i);
            return false;
        }
    }
    return true;
}

// Function to grant the request
void grantRequest(int process, int request[]) {
    for (int i = 0; i < NUM_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[process][i] += request[i];
        need[process][i] -= request[i];
    }
}

// Function to release resources when a process completes
void releaseResources(int process) {
    printf("\nProcess P%d has completed. Releasing resources...\n", process);
    for (int i = 0; i < NUM_RESOURCES; i++) {
        available[i] += allocation[process][i];
        allocation[process][i] = 0;  // Reset allocation
    }
    displayAvailableResources();
}

// Function to check if all processes are complete
bool allProcessesComplete() {
    for (int i = 0; i < NUM_PROCESSES; i++) {
        for (int j = 0; j < NUM_RESOURCES; j++) {
            if (need[i][j] != 0) {
                return false;
            }
        }
    }
    return true;
}

// Main function
int main() {
    calculateAndDisplayNeed();

    while (!allProcessesComplete()) {
        displayAvailableResources();

        printf("\nEnter process number (0-%d): ", NUM_PROCESSES - 1);
        int process;
        if (scanf_s("%d", &process) != 1 || process < 0 || process >= NUM_PROCESSES) {
            printf("Error: Invalid process number. Please try again.\n");
            while (getchar() != '\n'); // Clear invalid input
            continue;
        }

        printf("Enter resource request (A B C): ");
        int request[NUM_RESOURCES];
        bool validInput = true;
        for (int i = 0; i < NUM_RESOURCES; i++) {
            if (scanf_s("%d", &request[i]) != 1 || request[i] < 0) {
                validInput = false;
            }
        }
        if (!validInput) {
            printf("Error: Invalid resource request. Please try again.\n");
            while (getchar() != '\n'); // Clear invalid input
            continue;
        }

        if (canGrantRequest(process, request)) {
            grantRequest(process, request);
            printf("Request granted.\n");

            // Check if the process has completed and release resources if so
            bool processComplete = true;
            for (int i = 0; i < NUM_RESOURCES; i++) {
                if (need[process][i] != 0) {
                    processComplete = false;
                    break;
                }
            }

            if (processComplete) {
                releaseResources(process);
            }

        }
        else {
            printf("Request cannot be granted.\n");
        }

        calculateAndDisplayNeed();
    }

    printf("\nAll processes completed successfully.\n");
    return 0;
}