/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "fib.h"

int
main(int argc, char **argv)
{
  // Add any code you'd like to test out independently here. It will be
  // compiled and built into %_debug.run that you can load into gdb.
  printf("Fib(0) = %lu\n", fib(0));
  printf("Fib(1) = %lu\n", fib(1));

  return EXIT_SUCCESS;
}
