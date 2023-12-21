#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define max_line 255 // условный максимальный размер инпут строки

typedef struct list{
    char* val;
    struct list* next;
    struct list* head;
}list;

void list_free(list* l) {
    list* i = l->head;
    while (i != NULL){
        list* to_free = i;
        i = i->next;
        free(to_free);
    }
}

void push_back(list** l, char* val) {
    if ((*l)->val == NULL) {
        if (((*l)->val = (char*)malloc(strlen(val) + 1)) == NULL) {
            perror("malloc");
        }
        strcpy((*l)->val, val);
    }
    else {
        (*l)->next = (list*)malloc(sizeof(list));
        (*l)->next->head = (*l)->head;
        if (((*l)->next->val = (char*)malloc(strlen(val) + 1)) == NULL) {
            perror("malloc");
        }
        strcpy((*l)->next->val, val);
        (*l) = (*l)->next;
    }
}

list* init_list() {
    list* l = (list*)malloc(sizeof(list));
    l->head = l;
    l->next = NULL;
    l->val = NULL; 
    return l;
}

int main(){
    char input[max_line];
    list* l = init_list();

    while (input[0] != '.') {
        fgets(input, max_line, stdin);
        if (input[0] == '.')
            break;
        push_back(&l, input);
    }
    printf("----------------------\n");
    for (list* i = l->head; i != NULL; i = i->next){
        printf("%s", i->val);
    }
    exit(0);
}