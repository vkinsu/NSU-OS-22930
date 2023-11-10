#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char* line;
    struct Node* next;
} Storage;


Storage* newNode(char* input){

    size_t len = strlen(input);

    Storage* newLine = (Storage*)malloc(sizeof(Storage));

    newLine->line = (char*)malloc(len + 1);
    strcpy(newLine->line, input);
    newLine->line[len] = '\0'; 
    newLine->next = NULL; 
    return newLine;
}


void push(Storage **head, Storage **cur, Storage *newLine) {
    if (*head == NULL) {
        *head = newLine;
        *cur = newLine;
    }
    else {
        (*cur)->next = newLine;
        *cur = newLine;
    }
}

void printLines(Storage *head) { 
    printf("Lines:\n"); 
    Storage *cur = head; 
    while (cur != NULL) { 
        printf("%s", cur->line); 
        cur = cur->next; 
    } 
}

void freeLines(Storage *head) {
    Storage *cur = head;
    while (cur != NULL) {
        Storage *tmp = cur;
        cur = cur->next;
        free(tmp->line);
        free(tmp);
    }
}   
    

int main() {
    char input[100];
    struct Node* head = NULL;
    struct Node* current = NULL;

    printf("Enter a string (enter '.' to finish):\n");

    while (1) {

        fgets(input, sizeof(input), stdin);
        if (input[0] == '.') {
            break;
        }

        Storage *newLine = newNode(input);
        push(&head, &current, newLine);
    }

    printLines(head);
    freeLines(head);

    return 0;
}