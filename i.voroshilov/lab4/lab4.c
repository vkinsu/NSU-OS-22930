#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

typedef struct Node_s {
    char *string;
    struct Node_s *next;
} Node;

Node *head, *tail;

void init() {
    head = malloc(sizeof(Node));

    head->string = NULL;
    head->next = NULL;

    tail = head;
}

void push(char *string) {
    unsigned long len = strlen(string) + 1;
    char *copyPtr = calloc(len, sizeof(char));
    strcpy(copyPtr, string);

    tail->string = copyPtr;
    tail->next = calloc(1, sizeof(Node));

    tail = tail->next;
}

void printList() {
    Node *ptr = head;
    while (ptr != NULL) {
        if (ptr->string) {
            printf("%s\n", ptr->string);
        }
        ptr = ptr->next;
    }
}

int main() {
    char inputBuf[BUFFER_SIZE] = {0};

    init();

    while (fgets(inputBuf, BUFFER_SIZE, stdin) != NULL) {
        if (inputBuf[0] == '.') {
            printList();
            return 0;
        }

        char *lineEnd = strchr(inputBuf, '\n');
        if (lineEnd == NULL) {
            size_t newBufCnt = 0;
            size_t newBufCap = BUFFER_SIZE;
            char *newBuf = malloc(BUFFER_SIZE);

            memcpy(newBuf, inputBuf, BUFFER_SIZE);
            newBufCnt += BUFFER_SIZE - 1;

            while (fgets(inputBuf, BUFFER_SIZE, stdin) != NULL) {
                newBufCap += BUFFER_SIZE;
                newBuf = realloc(newBuf, newBufCap);

                memcpy(newBuf + newBufCnt, inputBuf, BUFFER_SIZE);
                newBufCnt += BUFFER_SIZE - 1;

                lineEnd = strchr(newBuf, '\n');
                if (lineEnd) {
                    *lineEnd = '\0';
                    push(newBuf);
                    free(newBuf);
                    break;
                }
            }
        } else {
            *lineEnd = '\0';
            push(inputBuf);
        }
    }

    return 0;
}