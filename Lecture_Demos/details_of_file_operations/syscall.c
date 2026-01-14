#include <fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
  int fd;

  fd = syscall(SYS_open, "test.txt", O_RDONLY);
  if(fd < 0) {
    perror("syscall:");
    return -1;
  }

  return 0;
}
