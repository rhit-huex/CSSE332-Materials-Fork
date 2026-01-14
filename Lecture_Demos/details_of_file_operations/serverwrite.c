#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int
main(int argc, char **argv)
{
  int sockfd = -1, listenfd = -1;
  struct sockaddr_in saddr;
  ssize_t n;
  char buff[4096];

  if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket:");
    return EXIT_FAILURE;
  }

  saddr.sin_family      = AF_INET;
  saddr.sin_port        = htons(8000);
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(listenfd, (struct sockaddr *)&saddr, sizeof saddr) < 0) {
    perror("bind");
    return EXIT_FAILURE;
  }

  if(listen(listenfd, 10) < 0) {
    perror("listen");
    return EXIT_FAILURE;
  }

  while(1) {
    sockfd = accept(listenfd, (struct sockaddr *)0, 0);
    write(sockfd, "Hello World!\n", strlen("Hello World!\n"));
    close(sockfd);
    close(listenfd);
    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}
