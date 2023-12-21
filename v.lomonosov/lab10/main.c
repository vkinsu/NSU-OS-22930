#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void executeCommand(int argc, char* argv[]) {
  if (argc == 1) {
    printf("Insufficient arguments for this program!\n");
    exit(EXIT_FAILURE);
  }

  pid_t childPid = fork();
  int status;

  switch (childPid) {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);
    case 0:
      execvp(argv[1], &argv[1]);
      exit(EXIT_FAILURE);
    default:
      waitpid(childPid, &status, 0);
      if (WIFEXITED(status))
        printf("Exit code of the child process: %d\n", WEXITSTATUS(status));
  }
}

int main(int argc, char* argv[]) {
  executeCommand(argc, argv);
  return 0;
}
