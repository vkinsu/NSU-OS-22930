#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

// limits: 2045+1 ('\0') simbols are visible, another simbols just throw out in my experience

typedef struct List_s
{
    int num;
    char** mas;
}List;

void init(List* list)
{
    list->num = 0;
    list->mas = (char**)malloc(sizeof(char*) * 2);
    if (list->mas == NULL)
    {
        perror("Failed allocating memory");
    }
    return;
}

void add(List* list, char* stroka)
{
    list->num = list->num + 1;
    list->mas = (char**)realloc(list->mas, sizeof(char*) * (list->num + 1));
    if (list->mas == NULL)
    {
        perror("Failed reallocating memory");
        return;
    }
    list->mas[list->num - 1] = (char*)malloc(sizeof(char) * (strlen(stroka) + 2));
    strcpy(list->mas[list->num - 1], stroka);
    
    return;
}

void print_free(List* list)
{
    for (int i = 0; i < list->num; i++)
    {
        printf("%s", list->mas[i]);
        free(list->mas[i]);
    }
    free(list->mas);
    return;
}

int main()
{
    List* list = (List*)malloc(sizeof(List));
    init(list);

    char mas[2050];
    while(1)
    {
        fgets(mas, 2048, stdin);
        add(list, mas);
        if (mas[0] == '.')
        {
            break;
        }
    }

    print_free(list);

    free(list);

    return 0;
}
