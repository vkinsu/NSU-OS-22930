#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
  pid_t process_id = fork();
  switch (process_id) {
    case -1:perror("fork");
      exit(1);
    case 0:printf(" CHILD: This is a descendant process!\n");
      printf(" CHILD: My PID is %d\n", getpid());
      printf(" CHILD: PID of my parent is %d\n", getppid());
      execlp("cat", "cat", "test.txt", NULL);
      printf(" CHILD: Exit!\n");
      exit(0);
    default:printf("PARENT: This is a parent process!\n");
      printf("PARENT: My PID is %d\n", getpid());
      printf("PARENT: PID of my descendant is %d\n", process_id);
      printf("PARENT: I'm waiting my descendant to call exit()...\n");
      wait(0);
      printf("PARENT: Exit!\n");
  }
  return 0;
}