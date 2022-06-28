#include "kernel/types.h"
#include "user/user.h"

void errorHandler(const char* str) {
  fprintf(2, str);
  exit(1);
}

int main() {
  int p2c[2], c2p[2];
  int parentpid, childpid;
  int pid;
  char buf;

  // Create two pipes for communication between parent and child processes
  if (pipe(p2c) == -1 || pipe(c2p) == -1) {
    errorHandler("pipe: create error!\n");
  }
  pid = fork();
  
  if (pid < 0) {
    errorHandler("fork: create error!\n");
  } else if (pid != 0) {  // parent
    close(p2c[0]);    // p2c write only
    close(c2p[1]);    // c2p read only

    parentpid = getpid();
    write(p2c[1], "g", 1);
    close(p2c[1]);

    read(c2p[0], &buf, 1);
    close(c2p[0]);
    fprintf(1, "%d: received pong\n", parentpid);
  } else {                // child
    close(p2c[1]);    // p2c read only
    close(c2p[0]);    // c2p write only

    childpid = getpid();
    read(p2c[0], &buf, 1);
    close(p2c[0]);

    write(c2p[1], "z", 1);
    close(c2p[1]);
    fprintf(1, "%d: received ping\n", childpid);
  }

  exit(0);
}
