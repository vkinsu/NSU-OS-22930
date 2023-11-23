#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_BUF_SIZE 100

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int file = open(argv[1], O_RDONLY);
    if (file == -1) {
        perror("Can't open file");
        return 2;
    }

    off_t offset = 0;
    int len = 0, num = 1;
    off_t end = lseek(file, 0, SEEK_END);
    lseek(file, 0, SEEK_SET);

    while (offset < end) {
        char c;
        read(file, &c, 1);
        offset++;
        len++;

        if (c == '\0') {
            break;
        } else if (c == '\n') {
            printf("Line %d: ", num);
            lseek(file, offset - len, SEEK_SET);

            char buf[MAX_BUF_SIZE];
            ssize_t bytesRead = read(file, buf, len - 1);

            if (bytesRead > 0) {
                buf[bytesRead] = '\0';
                printf("%s\n", buf);
            }

            len = 0;
            num++;
        }
    }

    close(file);

    return 0;
}
