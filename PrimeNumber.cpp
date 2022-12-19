#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>

int low;
int high;
int i;
int sentValue = -1;
char* filename[] = { "1001to100000.txt","100001to200000.txt","200001to300000.txt","300001to400000.txt","400001to500000.txt","500001to600000.txt","600001to700000.txt","700001to800000.txt","800001to900000.txt","900001to1000000.txt" };
int myPipe[10][2];

int primeNum(int low, int high, int* fd) {
    int i;
    int flag;
    char buffer[20];

    while (low < high)
    {
        flag = 0;

        for (i = 2; i <= low/2; ++i)
        {
            if (low % i == 0)
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {

            write(fd[1], &low, sizeof(low)); 

        }
        ++low;



    }
    write(fd[1], &sentValue, sizeof(sentValue)); 
}


int firstprime(int low, int high) {
    int i;
    int flag;
    char buffer[20];

    while (low < high)
    {
        flag = 0;

        for (i = 2; i <= low/2; ++i)
        {
            if (low % i == 0)
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {

            printf("%d\n", low);

        }
        ++low;



    }

}


int main() {

    pid_t pid;
    FILE* fd;
    int name;

    firstprime(2, 1000);
    fflush(stdout); 
    for (i = 0; i < 10; i++) {
        if (pipe(myPipe[i]) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < 10; i++)
    {
        
        pid = fork();
        if (pid == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {

            close(myPipe[i][0]); 

            if (i == 0)
            {

                primeNum(0, 1000, myPipe[i]);

                close(myPipe[i][1]); 
                exit(EXIT_SUCCESS);

            }
            else
            {

                primeNum(i * 1000 + 1, (i + 1) * 1000, myPipe[i]);  

                close(myPipe[i][1]);
                exit(EXIT_SUCCESS);
            }
        }

    }
      
    for (i = 0; i < 10; i++) {
        int buffer;
        name = open(filename[i], O_RDWR | O_CREAT, S_IRWXU); 

        char output[20];
        while (1) {
            read(myPipe[i][0], &buffer, sizeof(buffer)); 

            if (buffer == sentValue) {  
                break;
            }
            sprintf(output, "%d\n\0", buffer); 
            write(name, output, strlen(output)); 
            printf("%d\n\0", buffer);

        }
        close(name);
        close(myPipe[i][0]);
    }

}