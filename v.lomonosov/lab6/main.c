#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

typedef struct LineInfo {
  int indentation;
  int length;
} LineInfo;

typedef struct Vector {
  LineInfo *data;
  int capacity;
  int count;
} Vector;

int fileDescriptor;
Vector lineTable;

Vector vectorInit() {
  Vector v;
  v.data = (LineInfo *)malloc(sizeof(LineInfo));
  v.capacity = 1;
  v.count = 0;
  return v;
}

void vectorAdd(Vector *v, LineInfo *data) {
  if (v != NULL) {
    if (v->capacity == v->count) {
      v->capacity *= 2;
      LineInfo *check = (LineInfo *)realloc(v->data, v->capacity * sizeof(LineInfo));
      if (check != NULL)
        v->data = check;
      else
        return;
    }
    v->data[v->count++] = *data;
  }
}

void vectorDestroy(Vector *v) {
  if (v != NULL)
    free(v->data);
}

void buildTable() {
  lineTable = vectorInit();
  char symbol;
  int indentation = 0;
  int length = 0;
  while (1) {
    long bytesRead = read(fileDescriptor, &symbol, 1);
    if (symbol == '\n' || bytesRead == 0) {
      LineInfo data = {indentation, length};
      vectorAdd(&lineTable, &data);
      indentation += length + 1;
      length = 0;
      if (bytesRead == 0)
        break;
    } else
      length++;
  }
}

void printLine(long line) {
  if (line >= 0 && line < lineTable.count) {
    lseek(fileDescriptor, lineTable.data[line].indentation, SEEK_SET);
    char str[lineTable.data[line].length + 1]; // +1 for the null terminator
    read(fileDescriptor, str, lineTable.data[line].length);
    str[lineTable.data[line].length] = '\0'; // null-terminate the string
    printf("%s\n", str);
  } else {
    printf("Invalid line number\n");
  }
}

void signalHandler(int _) {
  printf("\n5 seconds is left!\n");
  printf("The content of the file:\n");
  for (int line = 0; line < lineTable.count; line++)
    printLine(line);
  exit(0);
}

int main() {
  fileDescriptor = open("data.txt", O_RDONLY);
  if (fileDescriptor == -1) {
    perror("Error opening file");
    exit(1);
  }
  buildTable();
  signal(SIGALRM, signalHandler);
  while (1) {
    printf("Enter the number of a line: ");
    char digits[32];
    alarm(5);
    if (fgets(digits, sizeof(digits), stdin) == NULL) {
      printf("Error reading input\n");
      break;
    }
    long lineNumber = strtol(digits, NULL, 10);
    if (lineNumber == 0)
      break;
    if (1 <= lineNumber && lineNumber <= lineTable.count)
      printLine(lineNumber - 1);
    else
      printf("Invalid line number\n");
  }
  vectorDestroy(&lineTable);
  close(fileDescriptor);
  return 0;
}