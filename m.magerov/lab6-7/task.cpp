#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int file_discryptor;
std::vector<int> endline;
std::string file_data;

void user_is_sleeping (int var){     
    lseek(file_discryptor, 0, SEEK_SET);
    char symb;
    std::cout << "\n5 SECONDS WITHOUT INPUT. PROGRAM IS SHUTTING DOWN...\n";
    while (read(file_discryptor, &symb, 1))
        std::cout << symb;
    std::cout << "\n";
    exit(0);
}


int main(int argv, char* argc[]) {
    if (argv != 2) {
        printf("you need to put your filepath into parameters of startup\n");
        return 1;
    }
    
    signal(SIGALRM, user_is_sleeping);

    std::cout << "try to open file " << argc[1] << "...\n";
    
    file_discryptor = open(argc[1], O_RDONLY);

    if (file_discryptor == -1) {
        std::cout << "smth went wrong with file " << argc[1] << "\n";
        exit(1);
    }

    std::cout << "file " << argc[1] << " has successfully opened!\n";

    std::cout << "Creating a string table...\n";

    endline.push_back(0);

    int cline = 0;
    char symb = 0;
    
    while (read(file_discryptor, &symb, 1)) {
        cline++;
        file_data += symb;
        if (symb == '\n' || symb == '\0')
            endline.push_back(cline);
    }
    int nline = 1;
    


    std::cout << "String table has created!\n";

    while (1) {
        //part of 6 task
        alarm(5);
        std::cout << "Type a line number what u need: ";
        std::cin >> nline;
        //part of 6 task
        alarm(0);
        if (nline == 0) break;
        
        if (nline > static_cast<int>(endline.size())) {
            std::cout << "Wrong line. Max line is: " << endline.size() << "\n";
            continue;
        }
        int cur_pointer = endline[nline - 1];
        while (cur_pointer < static_cast<int>(file_data.size())) {
            if (file_data[cur_pointer] == '\n') {
                break;
            }
            std::cout << file_data[cur_pointer];
            cur_pointer++;
        }

        std::cout << "\n";
    }
    return 0;
}