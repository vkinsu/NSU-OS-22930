#include <sys/ioctl.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#define BUF_SIZE 255

typedef struct terminal_d{
    char *data;
    int length;
    int cols;
    int size;
    int last_end;
} terminal_d;

struct termios default_settings;

void set_default_terminal_settingsgs(){
    tcsetattr(STDIN_FILENO, TCSANOW, &default_settings);
}

void terminal_init(terminal_d *terminal_d){
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    terminal_d->cols = 40;
    terminal_d->data = (char*)calloc(terminal_d->cols, 1);
    terminal_d->size = terminal_d->cols;
    terminal_d->length = 0;
    terminal_d->last_end = 0;
}


void terminal_write(terminal_d *terminal_d, char ch){
    if(!isprint(ch) && ch != '\n') return;
    if (ch == '\n')
        terminal_d->last_end = terminal_d->length;
    if (terminal_d->length > 0 && terminal_d->length + 1 - terminal_d->last_end > terminal_d->cols) {
        char next_line = '\n';
        write(STDOUT_FILENO, &next_line, sizeof(char));
        terminal_d->last_end = terminal_d->length;
    }
    while (terminal_d->length + 1 >= terminal_d->size){
        terminal_d->data = (char*)realloc(terminal_d->data, terminal_d->size + terminal_d->cols);
        terminal_d->size += terminal_d->cols;
    }
    write(STDOUT_FILENO, &ch, sizeof(char));
    terminal_d->data[terminal_d->length++] = ch;
}

void terminal_kill(terminal_d *terminal_d){
    if(terminal_d->length == 0) return;
    while(terminal_d->length > 0 && terminal_d->data[terminal_d->length] != '\n'){
        terminal_d->length--;
        write(STDOUT_FILENO, "\b \b", sizeof(char) * 3);
    }
}

void terminal_erase(terminal_d *terminal_d){
    if(terminal_d->length == 0) return;
    terminal_d->length--;
    write(STDOUT_FILENO, "\b \b", sizeof(char) * 3);
}

void terminal_erase_word(terminal_d *terminal_d){
    if(terminal_d->length == 0) return;
    while(terminal_d->length > 1 && isspace(terminal_d->data[terminal_d->length - 1])){
        terminal_d->length--;
        write(STDOUT_FILENO, "\b \b", sizeof(char) * 3);
    }
    while(terminal_d->length > 0 && !isspace(terminal_d->data[terminal_d->length - 1])){
        terminal_d->length--;
        write(STDOUT_FILENO, "\b \b", sizeof(char) * 3);
    }
}

void terminal_close(){
    set_default_terminal_settingsgs();
    char buf[] = "\nStopping the program...\n\000";
    write(STDOUT_FILENO, buf, sizeof(char) * strlen(buf));
    exit(0);
}

void type_kill(){
    char c = 21;
    ioctl(STDIN_FILENO, TIOCSTI, &c);
}

int main() {
    terminal_d terminal_d;
    terminal_init(&terminal_d);
    struct termios terminal_settings;
    tcgetattr(STDIN_FILENO, &terminal_settings);
    default_settings = terminal_settings;
    terminal_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW,  &terminal_settings);

    char ch;
    while (1){
        if(read(fileno(stdin), &ch, sizeof(char)) == -1){
            perror("Something goes wrong!\nStopping...\n");
            exit(1);
        }
        else if(terminal_settings.c_cc[VWERASE] == ch) terminal_erase_word(&terminal_d);//CTRL+W
        else if(terminal_settings.c_cc[VEOF] == ch) terminal_close();                   //close
        else if(terminal_settings.c_cc[VKILL] == ch) terminal_kill(&terminal_d);        //CTRL+U
        else if(terminal_settings.c_cc[VERASE] == ch) terminal_erase(&terminal_d);      //backspace
        else if('\n' == ch) terminal_write(&terminal_d, '\n');                          //enter
        else if(isprint(ch)) terminal_write(&terminal_d, ch);                           //any symbol
        else write(STDOUT_FILENO, "\a", sizeof(char));                                  //no printable symbol(writing alarm symbol)
    }
    set_default_terminal_settingsgs();
    return 0;
}
