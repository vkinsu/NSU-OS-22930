
#include <sys/stat.h>
#include <sys/mman.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int file_descriptor;
int line_number;
int num_lines = 1;
off_t line_offsets[100];
int line_lengths[100];
char *file_content;

void print_line(int line_num) {
    for (int i = line_offsets[line_num]; i < line_offsets[line_num] + line_lengths[line_num]; i++) {
        putchar(file_content[i]);
    }
    putchar('\n');
}

void handle_signal(int signal) {
    for (int j = 1; j <= num_lines; j++) {
        print_line(j);
    }
    if (signal == SIGALRM) {
        exit(EXIT_SUCCESS);
    } else {
        exit(EXIT_FAILURE);
    }
}

void read_file(const char *filename) {
    file_descriptor = open(filename, O_RDONLY);
    if (file_descriptor == -1) {
        perror("Can't open file");
        exit(EXIT_FAILURE);
    }

    struct stat file_stat;
    if (fstat(file_descriptor, &file_stat) == -1) {
        perror("Can't get file size");
        exit(EXIT_FAILURE);
    }

    file_content = (char *)mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, file_descriptor, 0);
    if (file_content == MAP_FAILED) {
        perror("Can't map");
        exit(EXIT_FAILURE);
    }

    off_t offset = 0;
    int length = 0;

    while (offset < file_stat.st_size) {
        offset++;
        length++;

        if (file_content[offset] == '\0' || file_content[offset] == '\n') {
            line_offsets[num_lines] = offset - length;
            line_lengths[num_lines] = length;
            length = 0;
            offset++;
            num_lines++;
        }
    }

    line_offsets[num_lines] = offset - length;
    line_lengths[num_lines] = length;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    read_file(argv[1]);

    struct sigaction signal_action;
    signal_action.sa_handler = handle_signal;
    sigemptyset(&signal_action.sa_mask);
    signal_action.sa_flags = 0;

    sigaction(SIGALRM, &signal_action, NULL);
    sigaction(SIGINT, &signal_action, NULL);

    alarm(5);

    scanf("%d", &line_number);

    while (line_number != 0) {
        print_line(line_number);
        scanf("%d", &line_number);
    }

    munmap(file_content, line_offsets[num_lines] + line_lengths[num_lines]);
    close(file_descriptor);

    return EXIT_SUCCESS;
}
