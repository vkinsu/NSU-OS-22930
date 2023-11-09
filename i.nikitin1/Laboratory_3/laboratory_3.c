#include <stdio.h>
#include <unistd.h>

int main() {

    // реальный идентификатор пользователя (или группы) сообщает,
	// кто создал процесс, а эффективный идентификатор пользователя 
	// (или группы) сообщает от чьего лица выполняется процесс, 
	// если эта информация изменяется.
    printf("Реальный ID пользователя: %d\n", getuid());
    printf("Эффективный ID пользователя: %d\n\n", geteuid());
    
    FILE* file = fopen("test.txt", "r");
    if (file) {
        printf("Файл открыт успешно.\n\n");
        fclose(file);
    }
    else {
        perror("Ошибка открытия файла!");
        printf("\n");
    }

    // match real and effective user ids
    setuid(geteuid());

    printf("Реальный ID пользователя: %d\n", getuid());
    printf("Эффективный ID пользователя: %d\n\n", geteuid());

    return 0;
}