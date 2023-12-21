#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <map>

using namespace std;

map <int, off_t> table_offset;
map <int, int> table_len;
int num = 1;
int file;
int n;

void take_tim(int a)
{
    for (int j = 1; j <= num; j++)
    {
        lseek(file, table_offset[j], 0);
        for (int i = 0; i < table_len[j]; i++)
        {
            char c;
            read(file, &c, 1);
            cout << c;
        }
        cout << endl;
    }
    exit(a);
}

void func(int b)
{
    while (n != 0)
    {
        cin >> n;
        lseek(file, table_offset[n], 0);
        for (int i = 0; i < table_len[n]; i++)
        {
            char c;
            read(file, &c, 1);
            cout << c;
        }
        cout << endl;
    }
}

int main(int argc, char* argv[])
{
    file = open(argv[1], O_RDONLY, 'r');
    if (file == -1)
    {
        perror("Can't open file");
        exit(10);
    }
    off_t offset = 0;
    int len = 0;

    off_t end = lseek(file, 0, SEEK_END);
    lseek(file, 0, 0);
    while (offset < end)
    {
        char c;
        read(file, &c, 1);
        offset++;
        len++;

        if (c == '\0')
        {
            break;
        }
        else if (c == '\n')
        {
            table_offset.emplace(num, offset - len);
            table_len.emplace(num, len - 1);
            len = 0;
            num++;
        }
    }
    table_offset.emplace(num, offset - len);
    table_len.emplace(num, len);

    struct sigaction sa;

    sa.sa_handler = take_tim;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);

    alarm(5);

    cin >> n;
    sa.sa_handler = func;
    sigaction(SIGALRM, &sa, NULL);

    while (n != 0)
    {
        lseek(file, table_offset[n], 0);
        for (int i = 0; i < table_len[n]; i++)
        {
            char c;
            read(file, &c, 1);
            cout << c;
        }
        cout << endl;
        cin >> n;
    }
}
