#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE_LENGTH 40

void disable_canonical_echo_mode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);

    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void ring_bell() {
    putchar('\a');
    fflush(stdout);
}

int main() {
    disable_canonical_echo_mode();

    char line[MAX_LINE_LENGTH + 1];
    char word[MAX_LINE_LENGTH + 1];
    int line_pos = 0;
    int word_pos = 0;

    printf("Enter text (Ctrl-D to exit):\n");

    while (1) {
        char c = getchar();

        if (c == EOF && line_pos == 0) {
            break;
        } else if (c == EOF) {
            ring_bell();
        } else if (c == '\n') {
            line[line_pos] = '\0';
            printf("%s\n", line);
            memset(line, 0, sizeof(line));
            line_pos = 0;
            word_pos = 0;
        } else if (c == '\b' || c == 127) { // Backspace or Delete
            if (line_pos > 0) {
                line_pos--;
                putchar('\b');
                putchar(' ');
                putchar('\b');
            } else {
                ring_bell();
            }
        } else if (c == 23) { // Ctrl-W
            while (line_pos > 0 && line[line_pos - 1] == ' ') {
                line_pos--;
                putchar('\b');
                putchar(' ');
                putchar('\b');
            }

            while (line_pos > 0 && line[line_pos - 1] != ' ') {
                line_pos--;
                putchar('\b');
                putchar(' ');
                putchar('\b');
            }
        } else if (c == 4 && line_pos == 0) { // Ctrl-D at the beginning of the line
            break;
        } else if (c == 4) { // Ctrl-D
            ring_bell();
        } else if (c == 21) { // Ctrl-U
            while (line_pos > 0) {
                line_pos--;
                putchar('\b');
                putchar(' ');
                putchar('\b');
            }
        } else {
            if (c == ' ' && word_pos > 0) {
                if (line_pos + word_pos > MAX_LINE_LENGTH) {
                    putchar('\n');
                    memset(line, 0, sizeof(line));
                    line_pos = 0;
                } else {
                    line[line_pos++] = ' ';
                }
                word_pos = 0;
            }

            if (line_pos < MAX_LINE_LENGTH) {
                putchar(c);
                line[line_pos++] = c;
                if (c != ' ') {
                    word[word_pos++] = c;
                }
            } else {
                ring_bell();
            }
        }

        fflush(stdout);
    }

    return 0;
}
