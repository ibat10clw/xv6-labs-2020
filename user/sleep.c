#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(2, "usage sleep [seconds]\n");
    }
    int seconds = atoi(argv[1]) * 10;
    sleep(seconds);
    exit(0);
}
