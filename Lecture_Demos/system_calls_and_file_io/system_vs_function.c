#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  int fd = openat(AT_FDCWD, "file.txt", O_WRONLY|O_CREAT|O_TRUNC,  S_IRWXU);

  int rc = write(fd, "hello world\n", 13);
  close(fd);

  int fd_read = openat(AT_FDCWD, "file.txt", 0);
  unsigned char buffer[13];
  // How many times is the system call read called?
  for(int i=0; i<13; i++){
    int read_bytes = read(fd_read, buffer+i, 1);
  }
  close(fd_read);

  FILE* fd_fread = fopen("file.txt", "r");
  unsigned char buffer_fread[13];
  // How many times is C library function fread called?
  for(int i=0; i<13; i++){
    int read_bytes = fread(buffer_fread+i, sizeof(char), 1, fd_fread);
  }
  // But how many times was the *system call* read called?
  fclose(fd_fread);
  return 0;
}
