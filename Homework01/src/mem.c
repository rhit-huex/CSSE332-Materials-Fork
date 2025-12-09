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
#define HLEN (2 * sizeof(int))

/**
 * Implementation of getmem()
 */
void *
getmem(int nc, int ni)
{
  int total_bytes = HLEN + (nc * 1) + (ni * 4);
  char* base_ptr = malloc(total_bytes);
  *((int*)base_ptr) = ni;
  *((int*)(base_ptr + sizeof(int)*1)) = nc;
  return base_ptr + HLEN;
}

/**
 * Implementation of freemem()
 */
void
freemem(void *mem)
{
  char* base_ptr = mem - HLEN;
  free(base_ptr);
}

/**
 * Implementation of getnc()
 */
int
getnc(void *mem)
{
  char* base_ptr = (char*)mem - HLEN;
  return *(int*)(base_ptr + sizeof(int)*1);
}

/**
 * Implementation of getni()
 */
int
getni(void *mem)
{
  char* base_ptr = (char*)mem - HLEN;
  return *(int*)base_ptr;
}

/**
 * Implementation of getstr()
 */
char *
getstr(void *mem)
{
  int ni = getni(mem);
  return (char*)mem + sizeof(int)*ni;
}

/**
 * Implementation of getintptr()
 */
int *
getintptr(void *mem)
{
  return (int*)mem;
}

/**
 * Implementation of getint_at()
 */
int
getint_at(void *mem, int idx, int *res)
{
  int ni = getni(mem);
  if(idx >= ni) { return -1; }
  int* int_arr = getintptr(mem);
  *res = int_arr[idx];
  return 0;
}

/**
 * Implementation of setint_at()
 */
int
setint_at(void *mem, int idx, int val)
{
  int ni = getni(mem);
  if(idx >= ni) { return -1; }
  int* int_arr = getintptr(mem);
  int_arr[idx] = val;
  return 0;
}

/**
 * Implementation of cpstr()
 */
size_t
cpstr(void *mem, const char *str, size_t len)
{
  char* str_start = getstr(mem);
  int nc = getnc(mem);
  if(len > nc) {
    memcpy(str_start, str, nc-1);
    str_start[nc - 1] = '\0';
    return nc;
  } else {
    memcpy(str_start, str, len);
    str_start[len] = '\0';
    return len+1; //add 1 for the null terminator
  }
  return 0;
}
