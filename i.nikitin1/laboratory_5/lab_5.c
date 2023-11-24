#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
    
#define MAX_LINE_LENGTH 100

typedef struct my_map_s {
  int index, cnt;
} my_map;


int main(int argc, char *argv[]) 
{ 

    assert(argc == 2);
 
    FILE *fp;

    if (!(fp = fopen(argv[1], "r"))) {
        perror("open error"); 
        exit(1); 
    }
 
    printf("Укажите строку:\n");
    int out_s;
    scanf("%d", &out_s);

    my_map table[1000];
    int n = 0;

    fseek(fp, 0L, SEEK_SET);
    int lc = 0;
    char ch;
    // Считываем слова из каждой строки
    for (n; !feof(fp); n++) 
    {
        table[n].index = lc;
        while (fread(&ch, 1, 1, fp)) 
        {
            if (ch == '\n')
            break;
            lc++;
        }
        table[n].cnt = lc - table[n].index;
        lc++;
    }

    assert((out_s != 0));
    assert((out_s - 1 < n));

    char *str = (char *) calloc(sizeof(char), table[out_s - 1].cnt + 1);
    fseek(fp, table[out_s - 1].index, SEEK_SET);
    fread(str, 1, table[out_s - 1].cnt, fp);

    printf("Номер линии %d -> :\n\"%s\"\n", out_s, str);


    return 0; 
}
