#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>

using namespace std;

int error = 34;
int code;

int main(int argc, char* argv[])
{
    map <int, off_t> table_offset;
    map <int, int> table_len;
    char buf[100];
    int file;
    file = open(argv[1], O_RDONLY, 'r');
    if (file == -1)
    {
        perror("Can't open file");
        exit(10);
    }
    off_t offset = 0;
    int len = 0, num = 1;

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

    int n;
    cin >> n;
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
