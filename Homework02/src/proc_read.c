/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/

#include "proc_read.h"
#include <errno.h>
#include <stdlib.h>

static void *
__get_ptr_from_str(const char *str)
{
  unsigned long long addr;

  errno = 0;
  addr  = strtoull(str, 0, 16);
  if(errno) {
    perror("strtoull: ");
    exit(EXIT_FAILURE);
  }
  return (void *)addr;
}

int
open_pmaps_file(struct program_info *pinfo, pid_t pid)
{
  // TODO: REMOVE THIS BEFORE STARTING, THIS IS TO SILENCE COMPILER WARNINGS
  (void)__get_ptr_from_str("fe");
  return 0;
}

int
parse_pmaps_file(struct program_info *pinfo)
{
  return 0;
}

void *
get_code_start(struct program_info *pinfo)
{
  return 0;
}

void *
get_code_end(struct program_info *pinfo)
{
  return 0;
}

void *
get_globals_start(struct program_info *pinfo)
{
  return 0;
}

void *
get_globals_end(struct program_info *pinfo)
{
  return 0;
}

void *
get_stack_start(struct program_info *pinfo)
{
  return 0;
}

void *
get_stack_end(struct program_info *pinfo)
{
  return 0;
}

void *
get_heap_start(struct program_info *pinfo)
{
  return 0;
}

void *
get_heap_end(struct program_info *pinfo)
{
  return 0;
}
