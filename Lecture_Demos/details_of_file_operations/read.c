#include <fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
  int fd;
  char buff;

  fd = syscall(SYS_open, "test.txt", O_RDONLY);
  if(fd < 0) {
    perror("syscall:");
    return -1;
  }

  for(int i = 0; i < 8192; i++)
    read(fd, &buff, 1);

  return 0;
}
