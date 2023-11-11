#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

typedef struct string_node {
    char *str;
    struct string_node *next;
} string_node_t;

string_node_t *init_string_node(const char *str) {
    string_node_t *node = (string_node_t *)malloc(sizeof(string_node_t));
    node->str = (char *)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(node->str, str);
    node->next = NULL;
    return node;
}

void free_string_node(string_node_t *node) {
    free(node->str);
    free(node);
}

void print_string_list(string_node_t *head) {
    string_node_t *cur = head;
    while (cur != NULL) {
        printf("%s", cur->str);
        cur = cur->next;
    }
}

void free_string_list(string_node_t *head) {
    string_node_t *cur = head;
    string_node_t *next = NULL;
    while (cur != NULL) {
        next = cur->next;
        free_string_node(cur);
        cur = next;
    }
}

int main() {
    char str[BUFFER_SIZE] = {0};
    string_node_t *head = NULL;
    string_node_t *cur = NULL;
    string_node_t *prev = NULL;
    while (fgets(str, BUFFER_SIZE, stdin) && str[0] != '.') {
        if (head == NULL) {
            head = init_string_node(str);
            prev = head;
        } else {
            cur = init_string_node(str);
            prev->next = cur;
            prev = cur;
        }
    }
    print_string_list(head);
    free_string_list(head);
    return 0;
}

