#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  int fd = openat(AT_FDCWD, "file.txt", O_WRONLY|O_CREAT|O_TRUNC,  S_IRWXU);
  int rc = write(fd, "hello world\n", 13);
  close(fd);
  return 0;
}
