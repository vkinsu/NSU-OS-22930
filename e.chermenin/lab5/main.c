#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct field_s {
  int pos, cnt;
} field;

int fill_table(FILE *fp, field *table, int n);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    perror(
        "invalid arguments\nIf you want to execute program you'll need to call it at form: ./programName \"filePath\"");
    return 0;
  }

  FILE *fp;

  if (!(fp = fopen(argv[1], "r"))) {
    perror("can't open file");
    return 0;
  }

  printf("please write number of line you want to print\n");
  int outStr;
  scanf("%d", &outStr);

  field table[1000];
  int n = 0;

  n = fill_table(fp, table, n);

  if (outStr == 0) {
    printf("Program was terminated by user\n");
    return 0;
  }

  if (outStr - 1 >= n) {
    perror("this string doesn't exists");
    return 0;
  }

  char *str = (char *) calloc(sizeof(char), table[outStr - 1].cnt + 1);
  fseek(fp, table[outStr - 1].pos, SEEK_SET);
  fread(str, 1, table[outStr - 1].cnt, fp);

  printf("the %d line in file is:\n\"%s\"\n", outStr, str);

  return 0;
}

int fill_table(FILE *fp, field *table, int n) {
  fseek(fp, 0L, SEEK_SET);
  int lc = 0;
  char ch;

  for (n; !feof(fp); n++) {
    table[n].pos = lc;
    while (fread(&ch, 1, 1, fp)) {
      if (ch == '\n')
        break;
      lc++;
    }
    table[n].cnt = lc - table[n].pos;
    lc++;
  }
  return n;
}