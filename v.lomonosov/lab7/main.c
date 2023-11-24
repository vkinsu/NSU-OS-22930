#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#define MAX_DIGIT_SIZE 32

typedef struct LineInfo {
  int indentation;
  int length;
} LineInfo;

typedef struct LineVector {
  LineInfo *data;
  int capacity;
  int count;
} LineVector;

LineVector lineTable;
char *fileData;
struct stat fileStat;

LineVector lineVectorInit() {
  LineVector v;
  v.data = (LineInfo *)malloc(sizeof(LineInfo));
  v.capacity = 1;
  v.count = 0;
  return v;
}

void lineVectorAdd(LineVector *v, LineInfo *newLine) {
  if (v != NULL) {
    if (v->capacity == v->count) {
      v->capacity *= 2;
      LineInfo *check = (LineInfo *)realloc(v->data, v->capacity * sizeof(LineInfo));
      if (check != NULL)
        v->data = check;
      else
        return;
    }
    v->data[v->count++] = *newLine;
  }
}

void lineVectorDestroy(LineVector *v) {
  if (v != NULL)
    free(v->data);
}

void buildLineTable() {
  lineTable = lineVectorInit();
  int indentation = 0;
  int length = 0;
  int offset = 0;
  for (; offset < fileStat.st_size; offset++) {
    if (fileData[offset] == '\n') {
      LineInfo newLine = {indentation, length};
      lineVectorAdd(&lineTable, &newLine);
      indentation += length + 1;
      length = 0;
    } else
      length++;
  }
}

void printLine(long line) {
  char lineStr[lineTable.data[line].length + 1];
  strncpy(lineStr, &fileData[lineTable.data[line].indentation], lineTable.data[line].length);
  lineStr[lineTable.data[line].length] = 0;
  printf("%s\n", lineStr);
}

void signalHandler(int _) {
  printf("\n5 seconds is left!\n");
  printf("The content of the file:\n");
  int line = 0;
  for (; line < lineTable.count; line++)
    printLine(line);
  munmap(fileData, fileStat.st_size);
  exit(0);
}

int main() {
  int fileDescriptor = open("data.txt", O_RDONLY);
  if (fileDescriptor == -1) {
    perror("open");
    return -1;
  }

  if (fstat(fileDescriptor, &fileStat) == -1) {
    perror("fstat");
    close(fileDescriptor);
    return -1;
  }

  fileData = (char *)mmap(0, fileStat.st_size, PROT_READ, MAP_SHARED, fileDescriptor, 0);
  if (fileData == MAP_FAILED) {
    perror("mmap");
    close(fileDescriptor);
    return -1;
  }

  buildLineTable();
  signal(SIGALRM, signalHandler);
  while (1) {
    printf("Enter the number of a line: ");
    char digits[MAX_DIGIT_SIZE];
    alarm(5);
    if (fgets(digits, MAX_DIGIT_SIZE, stdin) == NULL) {
      printf("Error reading input\n");
      break;
    }
    long lineNumber = strtol(digits, NULL, 10);
    if (lineNumber == 0)
      break;
    if (0 < lineNumber && lineNumber <= lineTable.count)
      printLine(lineNumber - 1);
    else
      printf("Invalid line number\n");
  }
  lineVectorDestroy(&lineTable);
  munmap(fileData, fileStat.st_size);
  close(fileDescriptor);
  return 0;
}