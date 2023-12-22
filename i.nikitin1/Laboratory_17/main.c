#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#define MAX_LENGTH 40

// Обработка символа Backspace
void handle_backspace(char *buffer, int *length) {
    if (*length > 0) {
        (*length)--;
        buffer[*length] = '\0';
        // Стереть последний символ на экране
        printf("\b \b"); 
        fflush(stdout);
    }
}

// Звуковой сигнал (CTRL-G)
void handle_ctrl_g() {
    printf("\a"); // Звуковой сигнал (CTRL-G)
    fflush(stdout); // немедленно вывести данные на экран из потока stdout
}

int main() {

    // Отключение канонического режима и вывода символов немедленно
    struct termios term; // структура termios, предназначенна для хранения параметров терминала.
    tcgetattr(STDIN_FILENO, &term); // получает текущие параметры терминала и сохраняет их в term
    term.c_lflag &= ~(ICANON | ECHO); // отключаем флаги канонического и эхо обработок
    tcsetattr(STDIN_FILENO, TCSANOW, &term);// получает новые параметры терминала и применяет сразу т.е."TCSANOW"
    //////////

    char buffer[MAX_LENGTH + 1]; // +1 -> для символа '\n'
    int length = 0;

    while (1) {
        char c;
        if (read(STDIN_FILENO, &c, 1) != 1) {
            term.c_lflag |= (ICANON | ECHO); // включаем обратно флаги канонического и эхо обработок
            tcsetattr(STDIN_FILENO, TCSANOW, &term); // применяем
            break; // выход, если нажата Ctrl-D
        }

        if (c == 4 && length == 0) {
            term.c_lflag |= (ICANON | ECHO); // включаем обратно флаги канонического и эхо обработок
            tcsetattr(STDIN_FILENO, TCSANOW, &term); // применяем
            break; // выход, если Ctrl-D нажат в начале строки
        }

        if (c == 127 || c == '\b') {
            // Обработка Backspace
            handle_backspace(buffer, &length);
        } else if (c == 23) {
            // Обработка CTRL-W
            while (length > 0 && buffer[length - 1] == ' ') {
                handle_backspace(buffer, &length);
            }
            while (length > 0 && buffer[length - 1] != ' ') {
                handle_backspace(buffer, &length);
            }
        } else if (c == 21) {
            // Обработка CTRL-U
            while (length > 0) {
                handle_backspace(buffer, &length);
            }
        } else if (c == '\n') {
            // Обработка Enter
            printf("\n");
            length = 0;
        } else if (c == 4) {
            // Обработка Ctrl-D
            break;
        } else {
            // Обработка обычного символа
            if (length == MAX_LENGTH) {
                printf("\n");
                length = 0;
            }
            buffer[length++] = c;
            putchar(c);
            fflush(stdout);
        }
    }

    return 0;
}