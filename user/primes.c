#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int);
/*
 * The first process feed all numbers to pipe, and output 2
 * Others process will filter 
 */

int main()
{
    int p[2];
    pipe(p);

    //close(p[0]);
    for (int i = 2; i <= 35; ++i) {
        write(p[1], &i, sizeof(int));
    }
    // close write side for read EOF   
    close(p[1]);

    sieve(p[0]);

    close(p[0]);
    
    exit(0);
}


void sieve(int fd) {
    //printf("pid: %d\n", getpid());
    int n;
    int p[2];

    // output a number and write number to next pipe if the number can't divide by n
    if (read(fd, &n, sizeof(int)) != 0) {
        pipe(p);
        printf("prime %d\n", n);
        int buf;
        while (read(fd, &buf, sizeof(int))) {
            if (buf % n != 0) {
                write(p[1], &buf, sizeof(int));
            }
        }
        close(p[1]);
    } else {
        exit(0);
    }

    close(fd);
    if (fork() == 0) {
        sieve(p[0]);
        close(p[0]);
    } else {
        close(p[0]);
        wait(0);
    }
    exit(0);
}