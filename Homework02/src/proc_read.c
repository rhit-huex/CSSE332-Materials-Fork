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
#include <stdio.h>
#include <string.h>

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
  char path[64];
  snprintf(path, sizeof(path), "~/../../proc/%d/maps", pid);

  FILE* fp = fopen(path, "r");
  if(NULL == fp) {
    perror("Open Failed");
    return -1;
  }
  pinfo->proc_file = fp;
  return 0;
}

/** PARSING INFORMATION:
 *  code start and end addr are on the text segment line where there is x perms for the prog.run
 *  globals start addr is the first address for the first line of text segment without x perms
 *  globals end addr is the last addr for the last line of text segment without x perms
 * We always have [heap], [vvar], [vdso], and [stack] so we can assume:
 * text segment -> [heap],heap segment -> [vvar] ... -> [stack],stack segment -> eof
 */
int
parse_pmaps_file(struct program_info *pinfo)
{
  char line[1024];
  FILE* fp = pinfo->proc_file; // read this file line by line
  char perms[5];
  unsigned int start = 0, end = 0;

  int code_seg_started = 0; // flag for the first line of code segment
  int globals_sed_started = 0; // flag for the first line of globals

  while(fgets(line, sizeof(line), fp)) {
    if(strstr(line, "[heap]") != NULL) { // we are on the heap line
      if(sscanf(line, "%x-%x", &start, &end) != 2) { continue; }
      pinfo->heap_start = start;
      pinfo->heap_end = end;
    }

    if(strstr(line, "[stack]") != NULL) {
      if(sscanf(line, "%x-%x", &start, &end) != 2) { continue; }
      pinfo->stack_start = start;
      pinfo->stack_end = end;
    }

    if(strstr(line, ".run") != NULL) {
      if(sscanf(line, "%x-%x %4s", &start, &end, perms) != 3) { continue; }
      if(strchr(perms, 'x')) { // this is the code line
        if(!code_seg_started) {
          pinfo->code_start = start;
          code_seg_started = 1;
        }
        pinfo->code_end = end;
      } else {
        if(!text_seg_started) {
          pinfo->globals_start = start;
          globals_seg_started = 1;
        }
        pinfo->globals_end = end;
      }
    }
  }
  fclose(pinfo->proc_file);
  pinfo->ready = 1;
  return 1;
}

void *
get_code_start(struct program_info *pinfo)
{
  if(pinfo) { return pinfo->code_start; }

  // Else parse it directly from self maps
  FILE* fp = fopen("~/../../proc/self/maps", "r");
  if (!fp) { return -1; }

  char line[1024];
  char perms[5];
  char exe_path[1024]; // for getting the path for the self maps
  unsigned int start = 0;
  unsigned int code_start = 0;

  ssize_t len = readlink("~/../../proc/self/exe", exe_path, sizeof(exe_path)-1);
  if(len == -1) { // something went wrong with getting the self path
    fclose(fp);
    return -1;
  }
  exe_path[len] = '\0';

  while(fgets(line, sizeof(line), fp)) {
    if(!strstr(line, exe_path)) { continue; } // if the line isn't in text segment
    if(sscanf(line, "%x-%x %4s", &start, &end, perms) != 3) { continue; }

    if(strchr(perms, 'x')) {
        code_start = start;
        break;
      }
    }
  }
  fclose(fp);
  return code_start;
}

void *
get_code_end(struct program_info *pinfo)
{
  if(pinfo) { return pinfo->code_end; }

  // Else parse directly from self maps
  FILE* fp = fopen("~/../../proc/self/maps", "r");
  if (!fp) { return -1; }
  char line[1024];
  char perms[5];
  unsigned int end = 0;
  unsigned int code_end = 0;

  char exe_path[1024];
  ssize_t len = readlink("~/../../proc/self/exe", exe_path, sizeof(exe_path)-1);
  if(len == -1) { fclose(fp); return -1; } // something went wrong with self path
  exe_path[len] = '\0';
e
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
