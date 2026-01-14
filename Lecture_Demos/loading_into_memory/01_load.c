#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
  char text[256] = {0};
  int fd;
  ssize_t len;

  fd = open("01_add.run", O_RDONLY);
  if(fd < 0) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  while((len = read(fd, text, 246)) < 0) {
    if(errno == EINTR)
      continue;
    perror("read");
    exit(EXIT_FAILURE);
  }

  int (*fn)(int, int) = (int (*)(int, int))text;
  printf("Calling add from the binary results in %d + %d = %d\n", 3, 2,
         fn(3, 2));
  exit(EXIT_SUCCESS);
}
