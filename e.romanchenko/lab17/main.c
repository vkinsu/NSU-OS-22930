#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <ctype.h>
#include <string.h>

#define LINE_LENGTH 40

struct termios original_termios;

void RawModeDisable() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

void handleErase(char *line, const int *len) {
    if (*len > 0) {
        line[*len - 1] = '\0';
        printf("\b \b");
    }
}

void handleKill(char *line) {
    line[0] = '\0';
    printf("\r\33[2K");
}

void handleCtrlW(char *line, const int *len) {
    int word_start = *len;
    for (int i = *len - 1; i >= 0; i--) {
        if (line[i] == ' ') {
            word_start = i;
            break;
        }
    }

    line[word_start] = '\0';
    printf("\33[%dD\33[K", *len - word_start);
}

int main() {
    tcgetattr(STDIN_FILENO, &original_termios);
    atexit(RawModeDisable);

    struct termios raw = original_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG); // Отключаем эхо, канонический режим и обработку сигналов
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    char c;
    static char inputLine[LINE_LENGTH + 1];
    int lineLength = 0;

    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (iscntrl(c) || !isprint(c)) {
            switch (c) {
                case CERASE:
                    handleErase(inputLine, &lineLength);
                    break;

                case CKILL:
                    handleKill(inputLine);
                    break;

                case CTRL('W'):
                    handleCtrlW(inputLine, &lineLength);
                    break;

                case CTRL('D'):
                    if (lineLength == 0) {
                        exit(0);
                    }
                    break;

                default:
                    putchar('\a');
                    break;
            }
        } else {
            if (lineLength == LINE_LENGTH) {
                putchar('\n');
                lineLength = 0;
            }

            inputLine[lineLength++] = c;
            inputLine[lineLength] = '\0';

            putchar(c);
        }

        fflush(NULL);
    }

    return 0;
}
