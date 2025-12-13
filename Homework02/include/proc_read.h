/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/
#ifndef PROC_READ_H
#define PROC_READ_H

#include <stdio.h>
#include <sys/types.h>

struct mem_region {
  void *start, *end;
};

struct program_info {
  pid_t pid; //!< The pid of the program we're checking for.
  int ready; //!< Boolean flag indicating whether parsing for this struct has
             //   completed.
};

/**
 * Open the /proc/<pid>/maps file for the process(program) requested.
 *
 * @param pinfo A structure program_info to update.
 * @param pid   The id of the program to open the maps file for.
 *
 * @return -1 on failure, 0 on success and updates pinfo.
 */
int open_pmaps_file(struct program_info *pinfo, pid_t pid);

/**
 * Parse a program information structure for a given program.
 *
 * @param pinfo   A program_info struct containing information about parsed
 *                things.
 *
 * @return 0 on failure, a valid program_info structure on success.
 */
int parse_pmaps_file(struct program_info *pinfo);

/**
 * Get the starting address of code section for the program.
 *
 * @param pinfo   An optional program_info structure to use.
 *
 * If `pinfo` is 0 (or NULL), then this function will get the starting address
 * of the code section of the currently running program (i.e., self).
 *
 * @return A valid address that is the start of the code section extraced from
 *    pinfo or for the currently running process.
 */
void *get_code_start(struct program_info *pinfo);

/**
 * Get the end address of code section for the program.
 *
 * @param pinfo   An optional program_info structure to use.
 *
 * If `pinfo` is 0 (or NULL), then this function will get the end address
 * of the code section of the currently running program (i.e., self).
 *
 * @return A valid address that is the end of the code section extraced from
 *    pinfo or for the currently running process.
 */
void *get_code_end(struct program_info *pinfo);

/**
 * Get the starting address of globals section for the program.
 *
 * @param pinfo   An optional program_info structure to use.
 *
 * If `pinfo` is 0 (or NULL), then this function will get the starting address
 * of the gloabls section of the currently running program (i.e., self).
 *
 * @return A valid address that is the start of the globals section extraced
 * from pinfo or for the currently running process.
 */
void *get_globals_start(struct program_info *pinfo);

/**
 * Get the end address of the globals section for the program.
 *
 * @param pinfo   An optional program_info structure to use.
 *
 * If `pinfo` is 0 (or NULL), then this function will get the end address
 * of the globals section of the currently running program (i.e., self).
 *
 * @return A valid address that is the end of the globals section extraced from
 *    pinfo or for the currently running process.
 */
void *get_globals_end(struct program_info *pinfo);

/**
 * Get the starting address of stack section for the program.
 *
 * @param pinfo   An optional program_info structure to use.
 *
 * If `pinfo` is 0 (or NULL), then this function will get the starting address
 * of the stack section of the currently running program (i.e., self).
 *
 * @return A valid address that is the start of the stack section extraced from
 *    pinfo or for the currently running process.
 */
void *get_stack_start(struct program_info *pinfo);

/**
 * Get the end address of stack section for the program.
 *
 * @param pinfo   An optional program_info structure to use.
 *
 * If `pinfo` is 0 (or NULL), then this function will get the end address
 * of the stack section of the currently running program (i.e., self).
 *
 * @return A valid address that is the end of the stack section extraced from
 *    pinfo or for the currently running process.
 */
void *get_stack_end(struct program_info *pinfo);

/**
 * Get the starting address of heap section for the program.
 *
 * @param pinfo   An optional program_info structure to use.
 *
 * If `pinfo` is 0 (or NULL), then this function will get the starting address
 * of the heap section of the currently running program (i.e., self).
 *
 * @return A valid address that is the start of the heap section extraced from
 *    pinfo or for the currently running process.
 */
void *get_heap_start(struct program_info *pinfo);

/**
 * Get the end address of heap section for the program.
 *
 * @param pinfo   An optional program_info structure to use.
 *
 * If `pinfo` is 0 (or NULL), then this function will get the end address
 * of the heap section of the currently running program (i.e., self).
 *
 * @return A valid address that is the end of the heap section extraced from
 *    pinfo or for the currently running process.
 */
void *get_heap_end(struct program_info *pinfo);

#endif
