#include "kernel/types.h"
#include "user/user.h"

void sieve(int pleft[]) {
    int p;
    int pright[2];

    close(pleft[1]);     // close pleft write
    if (read(pleft[0], &p, sizeof(int))) {
        fprintf(1, "prime %d\n", p);
        pipe(pright);
        if (fork() != 0) {
            close(pright[0]); // close pright read
            int buf;
            while(read(pleft[0], &buf, sizeof(int))) {
                if (buf % p) {
                    write(pright[1], &buf, sizeof(int));
                }
            }
            close(pleft[0]);  // close pleft read
            close(pright[1]); // close pright write;
            wait(0);
        } else {
            sieve(pright);
        }
    }
    exit(0);
}

int main() {
    int pinput[2];
    pipe(pinput);
    if (fork() != 0) {
        close(pinput[0]);   // close read
        for (int i = 2; i <= 35; i++) {
            write(pinput[1], &i, sizeof(int));
        }
        close(pinput[1]);   // close write
        wait(0);
    } else {
        sieve(pinput);
    }
    exit(0);
}
