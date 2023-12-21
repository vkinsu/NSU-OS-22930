#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int fileDescriptor;
std::vector<int> lineEnds;

void userIsSleeping(int signalNumber) {
    lseek(fileDescriptor, 0, SEEK_SET);
    char symbol;
    std::cout << "\n5 SECONDS WITHOUT INPUT. PROGRAM IS SHUTTING DOWN...\n";
    while (read(fileDescriptor, &symbol, 1))
        std::cout << symbol;
    std::cout << "\n";
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("You need to put your filepath into parameters of startup\n");
        return 1;
    }
    
    signal(SIGALRM, userIsSleeping);

    std::cout << "Try to open file " << argv[1] << "...\n";
    
    fileDescriptor = open(argv[1], O_RDONLY);

    if (fileDescriptor == -1) {
        std::cout << "Something went wrong with file " << argv[1] << "\n";
        exit(1);
    }

    std::cout << "File " << argv[1] << " has successfully opened!\n";

    std::cout << "Creating a string table...\n";

    lineEnds.push_back(0);

    int lineCounter = 0;
    char symbol = 0;
    
    while (read(fileDescriptor, &symbol, 1)) {
        lineCounter++;
        if (symbol == '\n' || symbol == '\0')
            lineEnds.push_back(lineCounter);
    }
    
    int lineNumber = 1;
    
    std::cout << "String table has been created!\n";
    
    while (1) {
        alarm(5);
        std::cout << "Type a line number that you need: ";
        std::cin >> lineNumber;
        alarm(0);

        if (lineNumber == 0) break;
        
        if (lineNumber > static_cast<int>(lineEnds.size())) {
            std::cout << "Wrong line. Max line is: " << lineEnds.size() << "\n";
            continue;
        }
        
        lseek(fileDescriptor, lineEnds[lineNumber - 1], SEEK_SET);
        
        while (read(fileDescriptor, &symbol, 1)) {
            if (symbol == '\n') {
                break;
            }
            std::cout << symbol;
        }

        std::cout << "\n";
    }
    return 0;
}
