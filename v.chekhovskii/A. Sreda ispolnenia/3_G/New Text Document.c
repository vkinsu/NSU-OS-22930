#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Real user id: %d\n", getuid());
    printf("Effective user id: %d\n\n", geteuid());
    
    FILE* f_file = fopen("file.txt", "r");
    if (f_file) {
        printf("File opened successfully.\n\n");
        fclose(f_file);
    }
    else {
        perror("Failed to open file");
        printf("\n");
    }

    // match real and effective user ids
    setuid(geteuid());
    printf("Real user id: %d\n", getuid());
    printf("Effective user id: %d\n\n", geteuid());

    return 0;
}
