#define MAX_STRING 100
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct Node {
    char* data;
    struct Node* next;
} My_Node;

int main()
{
    My_Node * head = NULL, * tail = NULL;
    char buffer[MAX_STRING];

    while (1) {
        fgets(buffer, 100, stdin);
       if (buffer[0] == '.') {
           break;
       }
       int len = strlen(buffer);
       char* str = (char*)malloc(len + 1); // +1 с учетом символа перехода /0
       strcpy(str, buffer);
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
    // Выводим данные списков...
    int cnt = 0;
    struct Node* render = head;
    while (render != NULL) {
        cnt++;
        printf("NODE №%d -> %s", cnt, render->data);
        render = render->next;
    }
    // Чистим память..
    render = head;
    while (render != NULL) {
        struct Node* temp = render;
        render = render->next;
        free(temp->data);
        free(temp);
    }

    return 0;
}