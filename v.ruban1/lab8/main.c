#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    const char *filename = "example.txt";
    int fd;

    if ((fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)) == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    struct flock file_lock;
    file_lock.l_type = F_WRLCK;
    file_lock.l_whence = SEEK_SET;
    file_lock.l_start = 0;
    file_lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &file_lock) == -1) {
        perror("Error locking file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (system("nano example.txt") == -1) {
        perror("Error invoking editor");
        file_lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &file_lock);
        close(fd);
        exit(EXIT_FAILURE);
    }

    file_lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &file_lock) == -1) {
        perror("Error unlocking file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    return 0;
}
