#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>

#define MAX_LINELENGTH 40



void writeCharacter(char c, int* lineLength, char* lineBuffer) {
    if (*lineLength >= MAX_LINELENGTH-1) {
        write(STDOUT_FILENO, "\n", sizeof(char));
        *lineLength = 0;
    }
    write(STDOUT_FILENO, &c, sizeof(c));
    lineBuffer[*lineLength] = c;
    (*lineLength)++;
}

void deleteCharacter(int* lineLength) {
    if (*lineLength != 0) {
        write(STDOUT_FILENO, "\b \b", sizeof(char)*3);
        (*lineLength)--;
    }
}



int main() {
    struct termios termiosCurrent, termiosDefaults;
    tcgetattr(STDIN_FILENO, &termiosCurrent);
    tcgetattr(STDIN_FILENO, &termiosDefaults);
    termiosCurrent.c_lflag &= ~(ICANON + ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &termiosCurrent);

    int lineLength = 0;
    char lineBuffer[MAX_LINELENGTH];

    while (1) {
        char c; // read input character
        read(fileno(stdin), &c, sizeof(char));

        if (isprint(c)) { // regular characters
            writeCharacter(c, &lineLength, lineBuffer);
        }
        else if (c == termiosCurrent.c_cc[VERASE]) { // erase
            deleteCharacter(&lineLength);
        }
        else if (c == termiosCurrent.c_cc[VKILL]) { // ctrl+u
            while (lineLength > 0) {
                deleteCharacter(&lineLength);
            }
            deleteCharacter(&lineLength);
        }
        else if (c == termiosCurrent.c_cc[VWERASE]) { // ctrl+w
            while (lineLength > 0 && !isspace(lineBuffer[lineLength-1])) {
                deleteCharacter(&lineLength);
            }
            if (isspace(lineBuffer[lineLength-1])) {
                deleteCharacter(&lineLength);
            }
        }
        else if (c == termiosCurrent.c_cc[VEOF]) { // ctrl+d
            if (lineLength == 0) {
                tcsetattr(STDIN_FILENO, TCSANOW, &termiosDefaults);
                exit(0);
            }
        }
        else { // error (unmapped unprintable character)
            write(STDOUT_FILENO, "\a", sizeof(char));
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &termiosDefaults);
    return 0;
}
