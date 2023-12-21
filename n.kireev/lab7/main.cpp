#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
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
char* address;

void take_tim(int a)
{
    for (int j = 1; j <= num; j++)
    {
        for (int i = table_offset[j]; i < table_offset[j] + table_len[j]; i++)
        {
            cout << address[i];
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
        for (int i = table_offset[n]; i < table_offset[n] + table_len[n]; i++)
        {
            cout << address[i];
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

    address = (char*)mmap((caddr_t)0, end + 100, PROT_READ, MAP_PRIVATE, file, offset);
    if (address == MAP_FAILED)
    {
        cerr << "Can't map";
        exit(-23);
    }

    while (offset < end)
    {
        offset++;
        len++;

        if (address[offset] == '\0')
        {
            break;
        }
        else if (address[offset] == '\n')
        {
            table_offset.emplace(num, offset - len);
            table_len.emplace(num, len);
            len = 0;
            offset++;
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
        for (int i = table_offset[n]; i < table_offset[n] + table_len[n]; i++)
        {
            cout << address[i];
        }
        cout << endl;
        cin >> n;
    }
}
