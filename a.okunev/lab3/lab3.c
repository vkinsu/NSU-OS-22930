#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    printf ("Real User ID = %d\n",getuid());
    printf ("Effective User ID = %d\n",geteuid());

    FILE *file = fopen("data.txt", "r");

    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    } else {
        fclose(file);
    }

    uid_t my_euid = geteuid();
    if (setuid(my_euid) == -1) {
        perror("Failed to set euid");    
    };

    printf ("Real User ID = %d\n",getuid());
    printf ("Effective User ID = %d\n",geteuid());

    file = fopen("data.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    } else {
        fclose(file);
    }

    return 0;
}