#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define NULL 0
#define BUFLEN 512
void xargs(char **arg_list) {
    if (fork() == 0) {
        exec(arg_list[0], arg_list);
    } else {
        wait(0);
    }
}
char **get_tokens(char *buf, int len) {
    static char *tokens[32] = {0};
    int j = 0;
    int n_tokens = 0;
    char token[32]= {0};

    for (int i = 0; i < len; ++i) {   
        if (buf[i] == '\n' || buf[i] == ' ')  {
            if (j == 0)
                continue;
            token[j] = '\0';
            j = 0;
            tokens[n_tokens] = (char *) malloc(sizeof(char) * strlen(token));
            strcpy(tokens[n_tokens++], token);
            memset(token, 0, sizeof(token));
        } else {
            token[j++] = buf[i];
        }
    }
    tokens[n_tokens]  = NULL;
    return tokens;
}
// note: exec will except find a NULL pointer in the end of arg list
int main(int argc, char *argv[]) {
    char buf[BUFLEN] = {0};
    char **tokens;
    char *arg_list[32] = {0};
    int len = read(0, buf, sizeof(buf));
    if (len < 0) {
        fprintf(2, "cannot read from std input\n");
        exit(-1);
    }
    
    for (int i = 1; i < argc; ++i) {
        arg_list[i-1] = argv[i];
    }
    // get all tokens from std input
    tokens = get_tokens(buf, len);
    // use token as additional argument for argv[1]
    for (int i = 0; tokens[i] != NULL; ++i) {
        printf("token %d: %s\n", i+1, tokens[i]);   
        arg_list[argc-1] = tokens[i];
        arg_list[argc] = NULL;
        xargs(arg_list);
        free(tokens[i]);
    }
    
    exit(0);
}


