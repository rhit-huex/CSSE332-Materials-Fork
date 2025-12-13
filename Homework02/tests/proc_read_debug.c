/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "proc_read.h"

int
main(int argc, char **argv)
{
  // Here's a sample way for you to use this API
  //
  struct program_info pinfo;
  void *start, *end;
  memset(&pinfo, 0, sizeof pinfo);

  open_pmaps_file(&pinfo, getpid());
  parse_pmaps_file(&pinfo);

  start = get_code_start(&pinfo);
  end   = get_code_end(&pinfo);
  printf("start = %p, end = %p\n", start, end);

  return EXIT_SUCCESS;
}
