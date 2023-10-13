#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char *str;
    struct Node *next;
};

struct Node *createNode(char *str) {
    struct Node *newNode = malloc(sizeof(struct Node));
    if (newNode == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
    newNode->str = malloc(strlen(str) + 1);
    if (newNode->str == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->str, str);
    newNode->next = NULL;
    return newNode;
}

void appendNode(struct Node **head, char *str) {
    struct Node *newNode = createNode(str);
    if (*head == NULL) {
        *head = newNode;
    } else {
        struct Node *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void printList(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        printf("%s", current->str);
        current = current->next;
    }
}

void freeList(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        struct Node *next = current->next;
        free(current->str);
        free(current);
        current = next;
    }
}

int main() {
    struct Node *head = NULL;
    char input[1000];

    printf("Enter strings (enter '.' to stop):");
    while (1) {
        fgets(input, sizeof(input), stdin);
        if (input[0] == '.') {
            break;
        }
        appendNode(&head, input);
    }

    printf("Strings entered:\n");
    printList(head);

    freeList(head);

    return 0;
}
