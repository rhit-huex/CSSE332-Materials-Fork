#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
  int sockfd = -1;
  struct sockaddr_in saddr;
  ssize_t n;
  char buff[4096];

  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket:");
    return EXIT_FAILURE;
  }

  saddr.sin_family = AF_INET;
  saddr.sin_port   = htons(8000);
  if(inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr) <= 0) {
    perror("inet_pton:");
    return EXIT_FAILURE;
  }

  if(connect(sockfd, (struct sockaddr *)&saddr, sizeof saddr) < 0) {
    perror("connect:");
    return EXIT_FAILURE;
  }

  printf("Received:\n");
  while((n = read(sockfd, buff, 4095)) > 0) {
    buff[n] = 0;
    printf("%ld: %s\n", n, buff);
  }

  close(sockfd);
  return EXIT_SUCCESS;
}
