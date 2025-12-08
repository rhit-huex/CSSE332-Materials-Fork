/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Implementation of the memory area with several types.
 *
 * @author Evan Hu
 * @date   12-8-2025
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"

// The length of the header we are using.
#define HLEN 2 * sizeof(int)

/**
 * Implementation of getmem()
 */
void *
getmem(int nc, int ni)
{
  // TODO: Add your code here...
  return 0;
}

/**
 * Implementation of freemem()
 */
void
freemem(void *mem)
{
  // TODO: Add your code here...
}

/**
 * Implementation of getnc()
 */
int
getnc(void *mem)
{
  // TODO: Add your code here...
  return -1;
}

/**
 * Implementation of getni()
 */
int
getni(void *mem)
{
  // TODO: Add your code here...
  return -1;
}

/**
 * Implementation of getstr()
 */
char *
getstr(void *mem)
{
  // TODO: Add your code here...
  return 0;
}

/**
 * Implementation of getintptr()
 */
int *
getintptr(void *mem)
{
  // TODO: Add your code here...
  return 0;
}

/**
 * Implementation of getint_at()
 */
int
getint_at(void *mem, int idx, int *res)
{
  // TODO: Add your code here...
  return -1;
}

/**
 * Implementation of setint_at()
 */
int
setint_at(void *mem, int idx, int val)
{
  // TODO: Add your code here...
  return -1;
}

/**
 * Implementation of cpstr()
 */
size_t
cpstr(void *mem, const char *str, size_t len)
{
  // TODO: Add your code here...
  return 0;
}
