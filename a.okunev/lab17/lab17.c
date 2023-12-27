#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 40
#define ERASE '\x7F'  // Символ стирания (Backspace)
#define KILL 0x15     // Символ KILL (Ctrl-U)
#define CTRL_W 0x17   // Символ CTRL-W
#define CTRL_D 0x04   // Символ CTRL-D
#define CTRL_G '\x07' // Звуковой сигнал

void disableCanonicalMode() {
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &new_termios);
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void enableCanonicalMode() {
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &new_termios);
    new_termios.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void processInput() {
    char line[MAX_LINE_LENGTH + 1];
    int position = 0;

    while (1) {
        char c;
        read(STDIN_FILENO, &c, 1);

        if (c == ERASE) {
            if (position > 0) {
                line[--position] = '\0';
                printf("\b \b");
            } else {
                printf("%c", CTRL_G);
            }
        } else if (c == KILL) {
            memset(line, '\0', 41);
            while (position>0){
                position--;
                line[position] = '\0';
                printf("\b \b");
            }
            fflush(stdout);
        } else if (c == CTRL_W) {
            while (position > 0 && line[position - 1] == ' ') {
                position--;
                line[position] = '\0';
                printf("\b \b");
            }

            while (position > 0 && line[position - 1] != ' ') {
                position--;
                line[position] = '\0';
                printf("\b \b");
            }
        } else if (c == CTRL_D) {
            if (position == 0) {
                break;
            } else {
                printf("%c", CTRL_G);
            }
        } else if (c >= 32 && c <= 126) {
                line[position++] = c;
                line[position] = '\0';
                printf("%c", c);
        }
        else {
            printf("%c", CTRL_G);
        }

        fflush(stdout);
    }
}

int main() {
    disableCanonicalMode();

    printf("Enter text (Ctrl-D to exit):\n");
    processInput();

    enableCanonicalMode();

    return 0;
}