#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE 255

typedef struct Node {
    char* data;
    struct Node* next;
    struct Node* head;
} Node;

void freeList(Node* list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* toFree = current;
        current = current->next;
        free(toFree->data);
        free(toFree);
    }
}

void appendToList(Node** list, char* data) {
    if ((*list)->data == NULL) {
        if (((*list)->data = (char*)malloc(strlen(data) + 1)) == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strcpy((*list)->data, data);
    } else {
        (*list)->next = (Node*)malloc(sizeof(Node));
        (*list)->next->head = (*list)->head;
        if (((*list)->next->data = (char*)malloc(strlen(data) + 1)) == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strcpy((*list)->next->data, data);
        (*list) = (*list)->next;
    }
}

Node* initializeList() {
    Node* list = (Node*)malloc(sizeof(Node));
    list->head = list;
    list->next = NULL;
    list->data = NULL;
    return list;
}

int main() {
    char input[MAX_LINE];
    Node* myList = initializeList();

    while (input[0] != '.') {
        fgets(input, MAX_LINE, stdin);
        if (input[0] == '.')
            break;
        appendToList(&myList, input);
    }

    printf("----------------------\n");

    for (Node* current = myList->head; current != NULL; current = current->next) {
        printf("%s", current->data);
    }

    freeList(myList);

    return 0;
}
