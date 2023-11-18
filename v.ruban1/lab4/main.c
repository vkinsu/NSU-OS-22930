#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 100

// Структура узла списка
struct Node {
    char *data;
    struct Node *next;
};

// Функция для вставки строки в список
void insertString(struct Node **head, const char *str) {
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Выделение памяти под строку и копирование данных
    newNode->data = (char *)malloc(strlen(str) + 1);
    if (newNode->data == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->data, str);

    // Вставка в начало списка
    newNode->next = *head;
    *head = newNode;
}

// Функция для вывода всех строк из списка
void printList(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }
}

// Функция для освобождения памяти, выделенной под список
void freeList(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        struct Node *temp = current;
        current = current->next;
        free(temp->data);
        free(temp);
    }
}

int main() {
    struct Node *head = NULL;
    char input[MAX_STRING_LENGTH];

    printf("Enter strings (enter '.' to finish):\n");

    // Ввод строк с клавиатуры
    while (1) {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';  // Удаляем символ новой строки

        if (strcmp(input, ".") == 0) {
            break;  // Завершение ввода при встрече точки
        }

        insertString(&head, input);
    }

    // Вывод строк из списка
    printf("\nStrings in the list:\n");
    printList(head);

    // Освобождение памяти
    freeList(head);

    return 0;
}
