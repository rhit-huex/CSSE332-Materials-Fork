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
#include <string.h>

#include "mem.h"

static int
run_valgrind(void)
{
  size_t len;
  int *p;
  int rc, res;
  char *msg = "Hello world! ";

  // 1. Allocate and free directly
  void *mem = getmem(1024, 1024);
  freemem(mem);

  // 2. Allocate and manipulate, then free
  mem = getmem(1024, 1024);
  len = cpstr(mem, msg, strlen(msg));
  if(!len) {
    fprintf(stderr, "ERROR: cpstr failed!\n");
    freemem(mem);
    exit(EXIT_FAILURE);
  }
  p       = getintptr(mem);
  p[0]    = -1;
  p[1000] = 3;
  rc      = getint_at(mem, 1000, &res);
  if(rc || res != 3) {
    fprintf(stderr, "ERROR: getint functions failed!");
    freemem(mem);
    exit(EXIT_FAILURE);
  }

  rc = getint_at(mem, 2000, &res);
  if(rc != -1) {
    fprintf(stderr, "ERROR: getint functions failed!");
    freemem(mem);
    exit(EXIT_FAILURE);
  }

  rc = setint_at(mem, 0, 19);
  if(rc) {
    fprintf(stderr, "ERROR: setint functions failed!");
    freemem(mem);
    exit(EXIT_FAILURE);
  }
  rc = setint_at(mem, 5000, 19);
  if(rc != -1) {
    fprintf(stderr, "ERROR: setint functions failed!");
    freemem(mem);
    exit(EXIT_FAILURE);
  }

  freemem(mem);

  // 3. Allocate only integers
  mem     = getmem(0, 1024);
  p       = getintptr(mem);
  p[0]    = -1;
  p[1000] = 3;
  rc      = getint_at(mem, 1000, &res);
  if(rc || res != 3) {
    fprintf(stderr, "ERROR: getint functions failed!");
    freemem(mem);
    exit(EXIT_FAILURE);
  }

  rc = getint_at(mem, 2000, &res);
  if(rc != -1) {
    fprintf(stderr, "ERROR: getint functions failed!");
    freemem(mem);
    exit(EXIT_FAILURE);
  }

  rc = setint_at(mem, 0, 19);
  if(rc) {
    fprintf(stderr, "ERROR: setint functions failed!");
    freemem(mem);
    exit(EXIT_FAILURE);
  }
  rc = setint_at(mem, 5000, 19);
  if(rc != -1) {
    fprintf(stderr, "ERROR: setint functions failed!");
    freemem(mem);
    exit(EXIT_FAILURE);
  }
  freemem(mem);

  // 4. Allocate only characters
  mem = getmem(1024, 0);
  len = cpstr(mem, msg, strlen(msg));
  if(!len) {
    fprintf(stderr, "ERROR: cpstr failed!\n");
    freemem(mem);
    exit(EXIT_FAILURE);
  }
  freemem(mem);

  printf("Everything finished...\n");
  return 0;
}

int
main(int argc, char **argv)
{
  // Use this to run valgrind and make sure that the memmory is being free'd up
  // correctly.
  run_valgrind();
  return EXIT_SUCCESS;
}
