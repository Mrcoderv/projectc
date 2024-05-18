#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TOURISTS 100
#define MAX_NAME_LEN 50
#define MAX_DEST_LEN 50

// Structure to hold tourist information
typedef struct {
    char name[MAX_NAME_LEN];
    char destination[MAX_DEST_LEN];
    int ticket_number;
} Tourist;

// Function to generate a random ticket number
int generate_ticket_number() {
    return rand() % 1000 + 1000;
}

// Function to delete a record by ticket number
void delete_record(const char *filename, int ticket_number) {
    FILE *temp_file;
    FILE *file;

    temp_file = fopen("temp.csv", "w");
    file = fopen(filename, "r");

    if (file == NULL || temp_file == NULL) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *token;
        int temp_ticket_number;
        char temp_name[MAX_NAME_LEN];
        char temp_destination[MAX_DEST_LEN];
        token = strtok(line, ",");
        strcpy(temp_name, token);

        token = strtok(NULL, ",");
        strcpy(temp_destination, token);

        token = strtok(NULL, ",");
        temp_ticket_number = atoi(token);

        if (temp_ticket_number != ticket_number) {
            fprintf(temp_file, "%s,%s,%d\n", temp_name, temp_destination, temp_ticket_number);
        }
    }
    fclose(file);
    fclose(temp_file);

    remove(filename);
    rename("temp.csv", filename);
}

// Function to view tourist data by ticket number
void view_record(const char *filename, int ticket_number) {
    FILE *file;
    file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    int found = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *token;
        int temp_ticket_number;
        char temp_name[MAX_NAME_LEN];
        char temp_destination[MAX_DEST_LEN];
        token = strtok(line, ",");
        strcpy(temp_name, token);

        token = strtok(NULL, ",");
        strcpy(temp_destination, token);

        token = strtok(NULL, ",");
        temp_ticket_number = atoi(token);

        if (temp_ticket_number == ticket_number) {
            found = 1;
            printf("Name: %s\n", temp_name);
            printf("Destination: %s\n", temp_destination);
            printf("Ticket Number: %d\n", temp_ticket_number);
            break;
        }
    }

    if (!found) {
        printf("Record not found for ticket number %d.\n", ticket_number);
    }

    fclose(file);
}

int main() {
    // Initialize random seed
    srand(time(NULL));

    // File pointer
    FILE *file;

    // Open file in write mode
    file = fopen("tourists.csv", "w");
    if (file == NULL) {
        printf("Error: Unable to create file.\n");
        return 1;
    }

    // Write headers
    fprintf(file, "Name,Destination,Ticket Number\n");

    // Array to hold tourist information
    Tourist tourists[MAX_TOURISTS];

    // Get tourist information
    int num_tourists;
    printf("Enter the number of tourists: ");
    scanf("%d", &num_tourists);
    if (num_tourists > MAX_TOURISTS) {
        printf("Error: Too many tourists\n");
        return 1;
    }

    // Populate tourist information and write to file
    for (int i = 0; i < num_tourists; i++) {
        printf("\nTourist %d:\n", i + 1);
        printf("Enter name: ");
        scanf("%s", tourists[i].name);
        printf("Enter destination: ");
        scanf("%s", tourists[i].destination);
        tourists[i].ticket_number = generate_ticket_number();
        printf("Ticket number: %d\n", tourists[i].ticket_number);

        // Write tourist information to file
        fprintf(file, "%s,%s,%d\n", tourists[i].name, tourists[i].destination, tourists[i].ticket_number);
    }

    // Close file
    fclose(file);

    printf("Tourist information saved to tourists.csv\n");

    // Operations
    char choice;
    int ticket_number;
    printf("\nDo you want to perform any operation? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        printf("Enter your choice:\n");
        printf("1. Delete a record by ticket number\n");
        printf("2. View tourist data by ticket number\n");
        printf("Choice: ");
        scanf(" %c", &choice);

        switch(choice) {
            case '1':
                printf("Enter the ticket number to delete: ");
                scanf("%d", &ticket_number);
                delete_record("tourists.csv", ticket_number);
                printf("Record with ticket number %d deleted successfully.\n", ticket_number);
                break;
            case '2':
                printf("Enter the ticket number to view data: ");
                scanf("%d", &ticket_number);
                view_record("tourists.csv", ticket_number);
                break;
            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}