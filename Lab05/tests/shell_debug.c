/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */

#include <stdio.h>
#include <stdlib.h>

#include "shell.h"

#ifndef MAX_SHELL_ARGS
#define MAX_SHELL_ARGS 16
#endif

int
main(int argc, char **argv)
{
  char *cmd = "echo hello";
  int carg  = 0;
  int i     = 0;
  char *varg[MAX_SHELL_ARGS];

  carg = generate_exec_args(cmd, varg);
  for(i = 0; i < carg; i++) {
    printf("argv[i] is %s\n", argv[i]);
  }
  printf("Last argument is %s\n", argv[i]);

  return EXIT_SUCCESS;
}
