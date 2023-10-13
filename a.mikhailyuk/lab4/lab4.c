#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char* data;
    struct Node* next;
};

int main() {
    struct Node* head = NULL;
    struct Node* tail = NULL;
    char input[100];

    while (1) {
        fgets(input, 100, stdin);
        if (input[0] == '.') {
            break;
        }
        int len = strlen(input);
        char* str = (char*)malloc(len + 1);
        strcpy(str, input);
        str[len] = '\0';
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->data = str;
        newNode->next = NULL;
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    struct Node* current = head;
    while (current != NULL) {
        printf("%s", current->data);
        current = current->next;
    }

    current = head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp->data);
        free(temp);
    }

    return 0;
}
