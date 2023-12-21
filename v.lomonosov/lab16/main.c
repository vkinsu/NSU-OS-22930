#include <curses.h>
#include <stdio.h>

int main() {
    initscr();
    cbreak();  // Разрешение немедленного ввода символов без буферизации
    noecho();  // Отключение автоматического вывода введенных символов

    while (1) {
        printw("Answer the question (Y/N), or press 'q' to quit: ");
        refresh();

        int ch = getch();

        if (ch == 'Q' || ch == 'q') {
            break;
        }
        else if (ch == 'Y' || ch == 'y') {
            printw("You answered 'Yes'\n");
        }
        else if (ch == 'N' || ch == 'n') {
            printw("You answered 'No'\n");
        }
        else {
            printw("Incorrect input\n");
        }
    }

    refresh();
    endwin();

    return 0;
}
