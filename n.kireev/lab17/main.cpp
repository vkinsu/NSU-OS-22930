#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main()
{
    struct termios* terma;
    tcgetattr(STDIN_FILENO, terma);
    terma->c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, terma);

    int MAX = 41;
    vector<string> str;   //  max len of str == 40 + '\n' or '\0'
    int pos = 0;

    while (1)
    {
        char c;
        c = getchar();

        if (c == '\x7F')      //  erase
        {
            if (pos > 0)
            {
                pos = pos - 1;
                str[str.size() - 1].pop_back();
                printf("\b \b");
            }
            else
            {
                if (str.size() > 0)
                {
                    str.pop_back();
                    if (str.size() == 0)
                    {
                        pos = 0;
                    }
                    else
                    {
                        pos = str[str.size() - 1].length();
                        struct winsize w;
                        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
                        for (int i = 0; i < w.ws_col - pos; i++)
                        {
                            printf("\b");
                        }
                    }
                }
            }
        }
        else if (c == 0x15) //  kill
        {
            if (str.size() > 0)
            {
                while (pos > 0)
                {
                    pos = pos - 1;
                    str[str.size() - 1].pop_back();
                    printf("\b \b");
                }
                str.pop_back();
            }
        }
        else if (c == 0x17)     //  ctrl-W
        {
            while (pos > 0 && !isspace(str[str.size() - 1][pos - 1]))
            {
                pos = pos - 1;
                str[str.size() - 1].pop_back();
                printf("\b \b");
            }
            while (pos > 0 && isspace(str[str.size() - 1][pos - 1]))
            {
                pos = pos - 1;
                str[str.size() - 1].pop_back();
                printf("\b \b");
            }
        }
        else if (c == 0x04)     //  ctrl-D
        {
            if (pos == 0)
            {
                break;
            }
        }
        else if (isprint(c))    //  normal symbol
        {
            if (str.size() == 0)
            {
                string strok;
                str.push_back(strok);
            }
            if (pos == 40)
            {
                printf("\n");
                string strok;
                pos = 0;
                str.push_back(strok);
            }
            str[str.size() - 1].push_back(c);
            pos = pos + 1;
            printf("%c", c);
        }
        else if (c == '\n')
        {
            if (str.size() == 0)
            {
                string strok;
                str.push_back(strok);
            }
            printf("\n");
            pos = 0;
            string strok;
            str.push_back(strok);
        }
        else                //  some noise      beep == '\a'
        {
            printf("\a%c", '\x07');
        }
        fflush(stdout);
    }
        
    terma->c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, terma);
}