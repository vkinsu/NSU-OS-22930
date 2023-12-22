#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <fcntl.h>

char myToupper(char a) {
	if (a >= 97) {
		return a - 32;
	}
	else return a;
}
 
int main(int argc, char** argv) { 
    int pipefd[2]; 
     
    // Создание канала 
    if (pipe(pipefd) == -1) { 
        perror("pipe"); 
        exit(EXIT_FAILURE); 
    } 
 
    pid_t pid = fork(); 
 
    if (pid == -1) { 
        perror("fork"); 
        exit(EXIT_FAILURE); 
    } else if (pid == 0) { 
    	close(pipefd[1]);

        char buf = 0;
        
        while (read(pipefd[0], &buf, 1) != 0) {
        	printf("%c", myToupper(buf));
        }

        close(pipefd[0]);  // Закрываем конец для записи 
    } 
    else { 
    	close(pipefd[0]);

    	int text_des = open("text", O_RDONLY);
    	char buf;

    	while(read(text_des, &buf, 1) != 0) {
    		write(pipefd[1], &buf, 1);
    	}

    	close(pipefd[1]);
    } 
 
    return 0; 
}