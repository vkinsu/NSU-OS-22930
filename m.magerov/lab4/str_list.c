#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define maxln 255 

typedef struct list{
    char* val;
    struct list* n;
    struct list* h;
}list;

void list_free(list* l) {
    list* i = l->h;
    while (i != NULL){
        list* to_free = i;
        i = i->n;
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
        (*l)->n = (list*)malloc(sizeof(list));
        (*l)->n->h = (*l)->h;
        if (((*l)->n->val = (char*)malloc(strlen(val) + 1)) == NULL) {
            perror("malloc");
        }
        strcpy((*l)->n->val, val);
        (*l) = (*l)->n;
    }
}

list* init_list() {
    list* l = (list*)malloc(sizeof(list));
    l->h = l;
    l->n = NULL;
    l->val = NULL; 
    return l;
}

int main(){
    char input[maxln];
    list* l = init_list();

    while (input[0] != '.') {
        fgets(input, maxln, stdin);
        if (input[0] == '.')
            break;
        push_back(&l, input);
    }
    printf("----------------------\n");
    for (list* i = l->h; i != NULL; i = i->n){
        printf("%s", i->val);
    }
    exit(0);
}