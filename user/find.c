#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void strcat(char *path, char *name) 
{
    char *p = path + strlen(path);
    *p++ = '/';
    memmove(p, name, strlen(name));
}

void find(char *path, char *target)
{
    // buf is relative path with cwd when program start
    char buf[128];
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
        case T_DIR:
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0) {
                    continue;
                }
                // de.name doesn't include path
                // we need to recover full path
                memset(buf, 0, sizeof(buf));
                strcpy(buf, path);
                strcat(buf, de.name);
                //printf("%s\n", buf);
                if (strcmp(target, de.name) == 0) {
                    printf("%s\n", buf);
                }
                // if find directory, then recursive search
                if (stat(buf, &st) >= 0 && st.type == T_DIR) {
                    if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
                        //printf("is dir %s\n", buf);
                        find(buf, target);
                    }
                }
            }
            break;
        case T_FILE:
            break;
        default:
            break;
    }
    close(fd);
    return;
}

int main(int argc, char *argv[])
{
    if(argc != 3){
        fprintf(2, "usage: find [path] [target]\n");
        exit(0);
    }

    find(argv[1], argv[2]);
    exit(0);
}
