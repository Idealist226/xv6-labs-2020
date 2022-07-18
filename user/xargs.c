#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char* argv[]) {
    int buf_idx, read_len;
    char buf[512];
    char* exe_argv[MAXARG];
     // copy execute args, skip "xargs" at argv[0]
    for (int i = 1; i < argc; i++) {
        exe_argv[i - 1] = argv[i];
    }
    while(1) {
        // read next line char until read '\n' or end
        buf_idx = -1;
        do {
            buf_idx++;
            read_len = read(0, &buf[buf_idx], sizeof(char));
        } while(read_len > 0 && buf[buf_idx] != '\n');
        if (read_len == 0 && buf_idx == 0) {
            break;
        }
        buf[buf_idx] = '\0';
        exe_argv[argc - 1] = buf;

        if (fork() == 0) {
            exec(exe_argv[0], exe_argv);
            exit(0);
        } else {
            wait(0);
        }
    }
    exit(0);
}
