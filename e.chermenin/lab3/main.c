#include <stdio.h>
#include <unistd.h>

void check_read_ability() {
  printf("%d %d\n", getuid(), geteuid());
  FILE* input = fopen("input.txt", "r");
  if (input == NULL)
    perror("ERROR");
  else
    printf("SUCESS\n");
}

int main() {
  check_read_ability();
  setuid(2);
  check_read_ability();
  return 0;
}
