#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
    
#define MAX_LINE_LENGTH 100

typedef struct my_map_s {
  int index, cnt;
} my_map;

FILE *fp,*fp2;
my_map table[1000];
char* argm;

void func_itter()
{
    //fp2 = fopen(argm, "r");

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

    for(int i=0;i<n;i++)
    {
        char *str = (char *) calloc(sizeof(char), table[i].cnt + 1);
        fseek(fp, table[i].index, SEEK_SET);
        fread(str, 1, table[i].cnt, fp);

        printf("Номер линии %d -> :\"%s\"\n", i+1, str);
        free(str);
    }
    exit(0);

}

void iterrupt()
{
    printf("\nВремя ожидания исткело!\n");
    func_itter();
}


int main(int argc, char *argv[]) 
{ 
    
    assert(argc == 2);
    argm = argv[1]; 
 
    if (!(fp = fopen(argv[1], "r"))) {
        perror("open error"); 
        exit(1); 
    }

    printf("Укажите строку: ");
    int out_s;

    signal(SIGALRM, iterrupt);
    alarm(5);
    scanf("%d", &out_s);

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

    printf("Номер линии %d -> :\"%s\"\n", out_s, str);
 
    return 0;  

}