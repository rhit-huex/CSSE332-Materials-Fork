#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * pipe_demo.c
 *
 * Task 1:
 * =======
 *  Follow along to understand this piece of code, run it to make sure you
 *  understand what it is doing.
 *  Ask questions if there is anything not clear.
 *
 * Task 2:
 * =======
 *  After completing task 1, add the following line:
 *      sleep(3);
 *  before the write call in the child. Answer the following question:
 *    What is the expected behavior given the rules for pipes?
 *
 */

int main(int argc, char **argv)
{
  int fd[2];
  pid_t pid;
  int nbytes;
  char readbuff[512];
  const char *msg = "My name is Aloy!";

  // DESCRIPTION:
  // ============
  // Our goal is to create a pipe between this parent and its child. By
  // definition, the parent MUST create the pipe so it can be shared with its
  // child (why can we not create it in the child?)

  // Step 1:
  //  Must call pipe(fd) BEFORE you call fork!
  if(pipe(fd) < 0) {
    perror("PANIC: pipe failed: ");
    exit(99);
  }

  // Step 2:
  //  Now we can fork.
  pid = fork();
  if(!pid) {
    // I am the child, I will be the WRITER

    // First thing you do as a writer is to CLOSE THE READING END!
    close(fd[0]);

    // Step 3:
    //  Write the message to be sent to whoever is listening.
    write(fd[1],    // the writing end of the pipe
        msg,        // the message to write
        strlen(msg) // the number of bytes to write
        );

    // Step 4:
    //  Done writing, MUST close the writing end.
    close(fd[1]);

    // Step 5:
    //  Leave, so should exit.
    exit(0);
  } else {
    // I am the parent, I will be the READER

    // First thing you do as a reader is to CLOSE THE WRITING END!
    close(fd[1]);

    // Step 6:
    //  Read from the pipe
    nbytes = read(fd[0], // where to read from
        readbuff,        // where to save the data read
        512);            // the maximum number of bytes to read

    if(nbytes) {
      printf("Parent (%d) read %d bytes from the child\n", getpid(), nbytes);

      // Be safe, always NULL terminate!
      if(nbytes < 512)
        readbuff[nbytes] = 0;
      printf("Parent (%d) read the following message from the child: %s\n",
          getpid(), readbuff);
    } else {
      fprintf(stderr, "ERROR: Parent (%d) could not read from the pipe\n",
          getpid());
    }
  }

  // Step 7:
  //  Done reading, MUST close the reading end!
  close(fd[0]);

  // done and leave
  exit(0);
}
