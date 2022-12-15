#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int p2c[2], c2p[2];
    char buf[128] = {0};
    pipe(p2c);
    pipe(c2p);

    if (fork() == 0) {
        // child
        // close duplicate fd
        close(p2c[1]);
        close(c2p[0]);

        char text[] = "pong";
        read(p2c[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);
        write(c2p[1], text, strlen(text));

        close(c2p[1]);
        close(p2c[0]);
    } else {
        // parent
        // close duplicate fd
        close(c2p[1]);
        close(p2c[0]);

        char text[] = "ping";
        write(p2c[1], text, strlen(text));
        read(c2p[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);

        close(c2p[0]);
        close(p2c[1]);
    }
    
    exit(0);
}
