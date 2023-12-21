#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void read_from_pipe(int file)
{
    FILE* stream;
    int c;
    stream = fdopen(file, "r");
    while ((c = fgetc(stream)) != EOF)
    {
        putchar(toupper(c));
    }
    fclose(stream);
}

void write_to_pipe(int file)
{
    FILE* stream;
    stream = fdopen(file, "w");
    fprintf(stream, "Den Namen im Kristall\n");
    fprintf(stream, "Erkennst Du nicht,\n");
    fprintf(stream, "Der Hass in deinem Blick\n");
    fprintf(stream, "Nimmt Dir die Sicht.\n");
    fprintf(stream, "Den wahren Namen\n");
    fprintf(stream, "Erkennt man erst dann,\n");
    fprintf(stream, "Wenn man auch\n");
    fprintf(stream, "ohne Augen sehen kann.\n");
    fclose(stream);
}

int main()
{
    pid_t pid;
    int mypipe[2];

    if (pipe(mypipe))
    {
        fprintf(stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }

    pid = fork();
    if (pid == (pid_t)0)
    {

        read_from_pipe(mypipe[0]);
        return EXIT_SUCCESS;
    }
    else if (pid < (pid_t)0)
    {
        fprintf(stderr, "Fork failed.\n");
        return EXIT_FAILURE;
    }
    else
    {
        write_to_pipe(mypipe[1]);
        return EXIT_SUCCESS;
    }
}