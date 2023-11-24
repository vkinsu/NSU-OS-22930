#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int file_discryptor;
std::vector<int> ends_of_lines;
//part of 6 task
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

    ends_of_lines.push_back(0);

    int line_counter = 0;
    char symb = 0;
    
    while (read(file_discryptor, &symb, 1)) {
        line_counter++;
        if (symb == '\n' || symb == '\0')
            ends_of_lines.push_back(line_counter);
    }
    int line_num = 1;
    
    std::cout << "String table has created!\n";
    while (1) {
        //part of 6 task
        alarm(5);
        std::cout << "Type a line number what u need: ";
        std::cin >> line_num;
        //part of 6 task
        alarm(0);
        if (line_num == 0) break;
        
        if (line_num > static_cast<int>(ends_of_lines.size())) {
            std::cout << "Wrong line. Max line is: " << ends_of_lines.size() << "\n";
            continue;
        }
        
        lseek(file_discryptor, ends_of_lines[line_num - 1], SEEK_SET);
        while (read(file_discryptor, &symb, 1)) {
            if (symb == '\n') {
                break;
            }
            std::cout << symb;
        }

        std::cout << "\n";
    }
    return 0;
}