#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#define CTRL_G '\x07' // Звуковой сигнал


volatile int signal_count = 0;


void set_terminal_mode() {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

int main() {
    set_terminal_mode();

    printf("Enter 'CTRL-G' to generate a sound. Enter q to see the count.\n");

    while (1) {
        char c;
        read(STDIN_FILENO, &c, 1);

        if (c== CTRL_G) {
                printf("%c", CTRL_G);
            fflush(stdout);
                signal_count++;
            } else if (c=='q') {
            printf("%d\n", signal_count);
            fflush(stdout);
            break;
        }
    }

    return 0;
}
