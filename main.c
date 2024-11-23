#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#define SIZE 50
#define PHONE 15

struct contact {
    char surname[SIZE];
    char lastname[SIZE];
    char phone_number[PHONE];

    struct contact *prev;
    struct contact *next;
};
void add_contact(char *surname, char *lastname, char *phone_number);
int remove_contact(char *surname, char *lastname); // Returns 1 if successful, 0 if not
int update_contact(char *surname, char *lastname);
void change_info(struct contact *con, int choice);
int search_contact(char *surname, char *lastname);
void list_contacts(void);
void ask_name(char *surname, char *lastname);
void free_contacts(void);


struct contact *head = NULL;  // Start of the contact list
struct contact *tail = NULL;  // End of the contact list

int main(void) {
    printf("--- Contact book ---\n");

    int input;
    do {
        printf("\n1. Add a new contact\n");
        printf("2. Remove a contact\n");
        printf("3. Update information on a contact\n");
        printf("4. Search for a contact\n");
        printf("5. List all contacts\n");
        printf("6. Exit\n");

        scanf("%d", &input);
        while((getchar()) != '\n');

        char surname[SIZE], lastname[SIZE], phone_number[PHONE];
        switch(input) {
        case 1:
            ask_name(surname, lastname);

            printf("Input phone number: ");
            if (fgets(phone_number, PHONE, stdin) == NULL) {
                printf("Error reading input or EOF encountered.\n");
                return;  // Handle error or exit function
            }
            phone_number[strcspn(phone_number, "\n")] = '\0';  // Remove newline if present

            add_contact(surname, lastname, phone_number);
            break;
        case 2:
            ask_name(surname, lastname);
            if(remove_contact(surname, lastname)) {
                printf("%s %s has been removed from the contact book.\n", surname, lastname);
            } else {
                printf("Could not remove %s %s (may not exist in the contact list)\n", surname, lastname);
            }
            break;
        case 3:
            ask_name(surname, lastname);
            if(update_contact(surname, lastname)) {
                printf("Contact info has been updated.\n");
            } else {
                printf("Could not update contact info.\n");
            }
            break;
        case 4:
            ask_name(surname, lastname);
            if(!search_contact(surname, lastname)) {
                printf("Could not find contact %s %s.\n", surname, lastname);
            }
            break;
        case 5:
            list_contacts();
            break;
        case 6:
            break;
        default:
            printf("Invalid input.\n");
            break;
        }
    } while(input != 6);

    free_contacts();

    return 0;
}

void add_contact(char *surname, char *lastname, char *phone_number) {
    struct contact *con = malloc(sizeof(struct contact));
    if (con == NULL) {
        printf("Memory allocation failed. Cannot add new contact.\n");
        return;
    }

    // Assign values
    strcpy(con->surname, surname);
    strcpy(con->lastname, lastname);
    strcpy(con->phone_number, phone_number);

    // Pointer for the new contact
    con->next = NULL;
    con->prev = NULL;

    // If list empty
    if(head == NULL) {
        head = con;
        tail = con;
        printf("Contact has been added.\n");
        return;
    }

    // Case 1: Insert before the current head
    if (strcmp(surname, head->surname) < 0) {
        con->next = head;
        head->prev = con;
        head = con;
        printf("Contact has been added.\n");
        return;
     }

    // Case 2: Insert at the correct position in the sorted list
    // curr will point to the contact next to the new contact
    struct contact *curr = head;
    while(curr != NULL && strcmp(surname, curr->surname) >= 0) {
        curr = curr->next;
    }

     if (curr == NULL) {
         // Case 2a: Insert at the end (after the current tail)
         con->prev = tail;
         tail->next = con;
         tail = con;
     } else {
         // Case 3b: Insert before the `curr` node
         con->next = curr;
         con->prev = curr->prev;
         if(curr->prev != NULL) {
            curr->prev->next = con;
         }
         curr->prev = con;
     }

     printf("Contact has been added.\n");
}

int remove_contact(char *surname, char *lastname) {
    if(head == NULL) {
        return 0; // Failed
    }

    // Find the contact to delete
    struct contact *curr = head;
    while(curr != NULL) {
        if(strcasecmp(curr->surname, surname) == 0 && strcasecmp(curr->lastname, lastname) == 0) {
            break;
        }
        curr = curr->next;
    }

    if (curr == NULL) {
        return 0;  // Contact not found
    }

    if(curr == head) { // First contact
        head = curr->next;
        if(head != NULL) {
            head->prev = NULL;
        } else { // Only contact
            tail = NULL;
        }
    } else if(curr == tail) { // last contact
        tail = curr->prev;
        if(tail != NULL) {
            tail->next = NULL;
        } else {
            head = NULL; // List is emtpy
        }
    } else { // Middle
        struct contact *prev = curr->prev;
        struct contact *next = curr->next;
        prev->next = next;
        next->prev = prev;
    }

    free(curr);  // Free the memory of the removed contact
    return 1; // Success
}

int update_contact(char *surname, char *lastname) {
    if(head == NULL) {
        return 0; // Failed
    }

    // Find the contact to delete
    struct contact *curr = head;
    while(curr != NULL) {
        if(strcasecmp(curr->surname, surname) == 0 && strcasecmp(curr->lastname, lastname) == 0) {
            break;
        }
        curr = curr->next;
    }

     if (curr == NULL) {
        return 0;  // Contact not found
    }

    int user_input;
    do {
        printf("What would you like to update on this contact?\n");
        printf("1. Change surname\n2. Change last name\n3. Change phone number\n4. Return to main menu\n");
        scanf("%d", &user_input);
        while ((getchar()) != '\n'); // Clear buffer

        if (user_input >= 1 && user_input <= 3) {
            change_info(curr, user_input);
        } else if (user_input != 4) {
            printf("Invalid input. Please try again.\n");
        }
    } while(user_input != 4);

    return 1; // Success
}

void change_info(struct contact *con, int choice) {
    const char *field_name;
    char new_info[SIZE];

    switch (choice) {
    case 1:
        field_name = "surname";
        break;
    case 2:
        field_name = "last name";
        break;
    case 3:
        field_name = "phone number";
        break;
    default:
        return; // Invalid choice
    }

    printf("Enter the new %s: ", field_name);
    fgets(new_info, SIZE, stdin);
    new_info[strcspn(new_info, "\n")] = '\0'; // Remove newline

    switch (choice) {
    case 1:
        strcpy(con->surname, new_info);
        break;
    case 2:
        strcpy(con->lastname, new_info);
        break;
    case 3:
        strcpy(con->phone_number, new_info);
        break;
    }
}

int search_contact(char *surname, char *lastname) {
    // Find the contact to display
    struct contact *curr = head;
    while(curr != NULL) {
        if(strcasecmp(curr->surname, surname) == 0 && strcasecmp(curr->lastname, lastname) == 0) {
            printf("\nFull contact information:\nSurname: %s\nLast name: %s\nPhone number: %s\n",
           curr->surname, curr->lastname, curr->phone_number);
            return 1; // Found
        }
        curr = curr->next;
    }

    return 0;
}

void list_contacts(void) {
    struct contact *curr = head;

    if(curr == NULL) {
        printf("No contacts in the contact book.\n");
        return;
    }

    printf("All contacts:\n");
    printf("%-15s%-15s%-15s\n", "Surname", "Last Name", "Phone Number"); // Column headers
    printf("---------------------------------------------\n");

    while (curr != NULL) {
        // Print each field with fixed-width formatting
        printf("%-15s%-15s%-15s\n", curr->surname, curr->lastname, curr->phone_number);
        curr = curr->next;
    }
}

void ask_name(char *surname, char *lastname) {
    printf("Input surname: ");
    if (fgets(surname, SIZE, stdin) == NULL || surname[0] == '\n') {
        printf("Invalid input for surname.\n");
        surname[0] = '\0';
        return;
    }
    surname[strcspn(surname, "\n")] = '\0';

    printf("Input last name: ");
    if (fgets(lastname, SIZE, stdin) == NULL || lastname[0] == '\n') {
        printf("Invalid input for last name.\n");
        lastname[0] = '\0';
    }
    lastname[strcspn(lastname, "\n")] = '\0';
}

void free_contacts(void) {
    struct contact *curr = head;
    while (curr != NULL) {
        struct contact *temp = curr;
        curr = curr->next;
        free(temp);
    }
    head = tail = NULL;
}
