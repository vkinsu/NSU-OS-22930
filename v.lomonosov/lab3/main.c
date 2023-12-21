#include <stdio.h>
#include <unistd.h>

void checkReadAbility() {
  printf("Real UID: %d, Effective UID: %d\n", getuid(), geteuid());

  FILE* input = fopen("input.txt", "r");
  if (input == NULL) {
    perror("ERROR");
  } else {
    printf("SUCCESS: Read permission granted\n");
    fclose(input);
  }
}

int main() {
  checkReadAbility();

  if (setuid(2) == -1) {
    perror("setuid");
    return 1;
  }

  printf("\nAfter setuid(2):\n");
  checkReadAbility();

  return 0;
}
