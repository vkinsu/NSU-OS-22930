#include <sys/ioctl.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 255

typedef struct TerminalData {
    char *buffer;
    int length;
    int columns;
    int size;
    int lastNewlineIndex;
} TerminalData;

struct termios defaultSettings;

void setDefaultTerminalSettings() {
    tcsetattr(STDIN_FILENO, TCSANOW, &defaultSettings);
}

void initializeTerminal(TerminalData *terminalData) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    terminalData->columns = 40;
    terminalData->buffer = (char *)calloc(terminalData->columns, 1);
    terminalData->size = terminalData->columns;
    terminalData->length = 0;
    terminalData->lastNewlineIndex = 0;
}

void writeTerminal(TerminalData *terminalData, char ch) {
    if (!isprint(ch) && ch != '\n')
        return;

    if (ch == '\n')
        terminalData->lastNewlineIndex = terminalData->length;

    if (terminalData->length > 0 && terminalData->length + 1 - terminalData->lastNewlineIndex > terminalData->columns) {
        char nextLine = '\n';
        write(STDOUT_FILENO, &nextLine, sizeof(char));
        terminalData->lastNewlineIndex = terminalData->length;
    }

    while (terminalData->length + 1 >= terminalData->size) {
        terminalData->buffer = (char *)realloc(terminalData->buffer, terminalData->size + terminalData->columns);
        terminalData->size += terminalData->columns;
    }

    write(STDOUT_FILENO, &ch, sizeof(char));
    terminalData->buffer[terminalData->length++] = ch;
}

void killLine(TerminalData *terminalData) {
    if (terminalData->length == 0)
        return;

    while (terminalData->length > 0 && terminalData->buffer[terminalData->length] != '\n') {
        terminalData->length--;
        write(STDOUT_FILENO, "\b \b", sizeof(char) * 3);
    }
}

void eraseCharacter(TerminalData *terminalData) {
    if (terminalData->length == 0)
        return;

    terminalData->length--;
    write(STDOUT_FILENO, "\b \b", sizeof(char) * 3);
}

void eraseWord(TerminalData *terminalData) {
    if (terminalData->length == 0)
        return;

    while (terminalData->length > 1 && isspace(terminalData->buffer[terminalData->length - 1])) {
        terminalData->length--;
        write(STDOUT_FILENO, "\b \b", sizeof(char) * 3);
    }

    while (terminalData->length > 0 && !isspace(terminalData->buffer[terminalData->length - 1])) {
        terminalData->length--;
        write(STDOUT_FILENO, "\b \b", sizeof(char) * 3);
    }
}

void closeTerminal() {
    setDefaultTerminalSettings();
    char message[] = "\nStopping the program...\n\000";
    write(STDOUT_FILENO, message, sizeof(char) * strlen(message));
    exit(0);
}

void simulateKill() {
    char c = 21;
    ioctl(STDIN_FILENO, TIOCSTI, &c);
}

int main() {
    TerminalData terminalData;
    initializeTerminal(&terminalData);

    struct termios terminalSettings;
    tcgetattr(STDIN_FILENO, &terminalSettings);
    defaultSettings = terminalSettings;
    terminalSettings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &terminalSettings);

    char ch;
    while (1) {
        if (read(fileno(stdin), &ch, sizeof(char)) == -1) {
            perror("Something went wrong!\nStopping...\n");
            exit(1);
        } else if (terminalSettings.c_cc[VWERASE] == ch)
            eraseWord(&terminalData);  // CTRL+W
        else if (terminalSettings.c_cc[VEOF] == ch)
            closeTerminal();  // Close
        else if (terminalSettings.c_cc[VKILL] == ch)
            killLine(&terminalData);  // CTRL+U
        else if (terminalSettings.c_cc[VERASE] == ch)
            eraseCharacter(&terminalData);  // Backspace
        else if ('\n' == ch)
            writeTerminal(&terminalData, '\n');  // Enter
        else if (isprint(ch))
            writeTerminal(&terminalData, ch);  // Any printable symbol
        else
            write(STDOUT_FILENO, "\a", sizeof(char));  // No printable symbol (write alarm symbol)
    }

    setDefaultTerminalSettings();
    return 0;
}
