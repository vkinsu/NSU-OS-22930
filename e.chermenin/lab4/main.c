#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char *string;

typedef struct Node {
  struct Node *ptr_next;
  string value;
} Node;

typedef Node *head;

head list_init() {
  Node *head = (Node *) calloc(1, sizeof(Node));
  return head;
}

void list_destroy(head node) {
  while (node) {
    Node *next = node->ptr_next;
    if (node->value) {
      free(node->value);
      node->value = NULL;
    }
    free(node);
    node = next;
  }
}

Node *list_add(Node *after, string value) {
  if (after) {
    Node *new;
    if (after->value) {
      new = (Node *) malloc(sizeof(Node));
      new->ptr_next = after->ptr_next;
      after->ptr_next = new;
    } else
      new = after;
    new->value = (string) malloc(strlen(value) + 1);
    strcpy(new->value, value);
    return new;
  }
  return NULL;
}

void read_to_list(Node *head) {
  Node *current = head;
  if (current) {
    string str = (string) malloc(2048);
    while (1) {
      fgets(str, 2048, stdin);
      if (str[0] == '.')
        break;
      current = list_add(current, str);
    }
    free(str);
  }
}

void print_from_list(Node *head) {
  Node *current = head;
  while (current) {
    printf("%s", current->value);
    current = current->ptr_next;
  }
}

int main() {
  Node *head = list_init();
  read_to_list(head);
  print_from_list(head);
  list_destroy(head);
  return 0;
}