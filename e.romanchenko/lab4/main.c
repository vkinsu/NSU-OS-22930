#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

typedef struct Node {
    struct Node* next;
    char* data;
} Node;

Node* first = NULL;
Node* last = NULL;

void addNode(Node* newNode) {
    newNode->next = NULL;
    if (first == NULL) {
        first = last = newNode;
    } else {
        last->next = newNode;
        last = newNode;
    }
}

void freeNodes() {
    while (first != NULL) {
        Node* current = first;
        first = first->next;

        free(current->data);
        free(current);
    }
}

int main() {
    char inputBuffer[BUFSIZ];

    while (fgets(inputBuffer, BUFSIZ, stdin) != NULL) {
        if (inputBuffer[0] == '.')
            break;

        size_t length = strlen(inputBuffer);

        Node* newNode = (Node*)malloc(sizeof(Node));
        if (newNode == NULL) {
            perror("Can't allocate memory");
            exit(ENOMEM);
        }

        newNode->data = (char*)malloc(length + 1);
        if (newNode->data == NULL) {
            perror("Can't allocate memory");
            exit(ENOMEM);
        }

        strcpy(newNode->data, inputBuffer);

        addNode(newNode);
    }

    printf("\n");

    Node* current = first;
    while (current != NULL) {
        printf("%s", current->data);
        current = current->next;
    }

    freeNodes();

    return 0;
}
