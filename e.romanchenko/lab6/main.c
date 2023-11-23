#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_BUF_SIZE 100

int file;
int n;
int num = 1;

off_t table_offset[100];
int table_len[100];

void print_line(off_t offset, int len) {
    lseek(file, offset, SEEK_SET);

    char buf[MAX_BUF_SIZE];
    ssize_t bytesRead = read(file, buf, len);

    if (bytesRead > 0) {
        buf[bytesRead] = '\0';
        printf("%s\n", buf);
    }
}

void handle_timeout(int signal) {
    off_t offset = 0;
    int len;

    lseek(file, 0, SEEK_SET);
    for (int j = 1; j < num; j++) {
        len = table_len[j];
        print_line(offset, len);
        offset += len;
    }

    exit(signal);
}

void handle_user_input(int signal) {
    while (n != 0) {
        print_line(table_offset[n], table_len[n]);
        scanf("%d", &n);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    file = open(argv[1], O_RDONLY);
    if (file == -1) {
        perror("Can't open file");
        exit(EXIT_FAILURE);
    }

    off_t offset = 0;
    int len;

    while (read(file, &len, sizeof(int))) {
        table_offset[num] = offset;
        table_len[num] = len;
        offset += len;
        num++;
    }

    struct sigaction timeout_sa, user_input_sa;

    timeout_sa.sa_handler = handle_timeout;
    sigemptyset(&timeout_sa.sa_mask);
    timeout_sa.sa_flags = 0;
    sigaction(SIGALRM, &timeout_sa, NULL);

    alarm(5);

    scanf("%d", &n);

    user_input_sa.sa_handler = handle_user_input;
    sigaction(SIGALRM, &user_input_sa, NULL);

    while (n != 0) {
        print_line(table_offset[n], table_len[n]);
        scanf("%d", &n);
    }

    close(file);

    return EXIT_SUCCESS;
}
