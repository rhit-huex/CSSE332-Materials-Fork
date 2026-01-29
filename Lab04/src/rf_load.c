/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "rf_load.h"
#include "rf_parse.h"

//typedef int(*)() start_func_t;
//  start_func_t start_fn = (start_func_t)entry_address;
//  int return_value = start_fn();

int
calc_npages(struct rf_parse_state *ps)
{
  // TODO:
  // =====
  //  Implement this function to compute how many pages we'd need to store the
  //  code and globals regions of the CSSE332RF binary in question.

  // Size of code? Size of globals? --> Calc #of pages to store (also 1 pg per section)
  int n_pages = 0;
  int page_size = getpagesize();
  struct rf_shdr code_hdr;
  if(rf_find_section_by_name(ps, &code_hdr, ".text") == 0) {
    int code_bytes = code_hdr.len;
    int code_pages = (code_bytes + page_size - 1) / page_size; // handles rounding
    n_pages += code_pages;
  }

  struct rf_shdr global_hdr;
  if(rf_find_section_by_name(ps, &global_hdr, ".data") == 0) {
    int global_bytes = global_hdr.len;
    int global_pages = (global_bytes + page_size - 1) / page_size;
    n_pages += global_pages;
  }
  ps->error = RF_ERR_NULL;
  return n_pages;
}

void *
rf_load_code(struct rf_parse_state *ps, void* code, int* len)
{
  // TODO:
  // =====
  //  Implement code that would load the code segment from the file into the
  //  area of memory starting at location code.

  // 1. Get the code header from ps
  struct rf_shdr code_hdr;
  int code_bytes = 0;
  int code_pages = 0;
  int page_size = getpagesize();

  if(rf_find_section_by_name(ps, &code_hdr, ".text") == 0) {
    code_bytes = code_hdr.len;
    code_pages = (code_bytes + page_size - 1) / page_size; // rounds to nearest pages
    //fprintf(stdout, "Code Pages: %d, Code_bytes: %d\n", code_pages, code_bytes);
  } else {
    perror("rf_find_section");
    ps->error = RF_ERR_SEC_NFOUND;
    return 0;
  }

  // 2. Use MAP_FIXED if we are provided a code pointer
  int mmap_flag = MAP_PRIVATE | MAP_ANONYMOUS;
  if(code != NULL) { mmap_flag |= MAP_FIXED; }

  // 3. Create an area of memory in the monitors address space for the code section
  void* p = 0;
  p = mmap(code, code_bytes, PROT_WRITE, mmap_flag, -1, 0);
  if(p == MAP_FAILED) {
    perror("mmap");
    // exit(EXIT_FAILURE);
    ps->error = RF_ERR_LASTONE;
    return 0;
  }

  // single star, bc we want to pass by reference
  // 4. Copy into our mmap region the section body
  unsigned char* buf = 0;
  if(rf_read_section_body(ps, &code_hdr, &buf) == 0) {
    memcpy(p, buf, code_bytes);
  } else {
    // perror("rf_read_section_body");
    // exit(EXIT_FAILURE);
    ps->error = RF_ERR_LASTONE;
    return 0;
  }
  //fwrite(buf, 1, code_bytes, stdout);
  //for(int i=0; i<code_bytes; i++) {
  //  fprintf(stdout, "%02x ", buf[i]);
  //  if ((i+1)%16 == 0) printf("\n");
  //}
  //fprintf(stdout, "Code Pages: %d, Code_bytes: %d\n", code_pages, code_bytes);
  int rc = mprotect(p, (code_pages * page_size), PROT_READ | PROT_EXEC);
  if(rc < 0) {
    // perror("mprotect");
    // exit(EXIT_FAILURE);
    ps->error = RF_ERR_LASTONE;
    return 0;
  }

  *len = code_bytes;

  return p; // return the spot in memory
}

int
rf_exec_code_only(const char *path, struct rf_exec_state *exst)
{
  int (*start_fn)();
  // TODO:
  // =====
  //  Implement code that loads and executes a process from a given binary
  //  path.
  struct rf_parse_state ps = {0};
  struct rf_hdr hdr        = {0};
  int err                  = 0;

  // Load the parser state
  if(rf_parse_state_init(&ps, path)) {
    return ps.error;
  }

  // Read and validate the header
  if(rf_read_hdr(&ps, &hdr) || rf_validate_hdr(&hdr, &ps)) {
    err = ps.error;
    rf_parse_state_destroy(&ps);
    return err;
  }

  // 1. Load the code segment from the binary.
  // Use our load function.
  // The starting code region is defined in exst->*code. Then get a pointer to exst->clen to set code length.
  int len = 0;
  void* p = rf_load_code(&ps, NULL, &len);
  // Set globals parts to 0 for now
  exst->data = 0; exst->dlen = 0;
  exst->clen = len; exst->code = p;

  // 2. Find the entry point for the newly created virtual process.
  // path is the start of the RF executable, so from that we need to find the address of start_func
  // Probably hdr->entry_offset
  //start_func_t start_fn = (start_func_t)(hdr->entry_offset);
  start_fn = (int (*)()) (p + hdr.entry_offset);

  // 3. Execute the process by simply calling the entry function and capturing
  //    the return value. Save the return value into `exst->rv`.
  int return_value = start_fn();
  exst->rv = return_value;
  // DO NOT unmap the execution state, we do that in rf_unmap_state
  rf_parse_state_destroy(&ps);
  return 0;
}

void *
rf_load_data(struct rf_parse_state *ps, void *code, int *len)
{
  // TODO:
  // =====
  //  Implement code that would load the globals segment (if any) from the file
  //  into the page that starts right after the code region. Make sure to stay
  //  page aligned.

  struct rf_shdr globals_hdr;

  int globals_bytes = 0;
  int globals_pages = 0;
  int page_size = getpagesize();

  // We assume that code points to the start of the mmapped region we just allocated
  // 1. Check for a globals header using rf_find_section_by_name
  //    If no globals header exists then nothing to do so exit
  if(rf_find_section_by_name(ps, &globals_hdr, ".data") == 0) {
    globals_bytes = globals_hdr.len;
    globals_pages = (globals_bytes + page_size - 1) / page_size; // rounds to nearest pages
    //fprintf(stdout, "Globals Pages: %d, Globals Bytes: %d\n", globals_pages, globals_bytes);
  } else {
    ps->error = RF_ERR_SEC_NFOUND;
    return 0;
  }

  // 2. mmap more space
  int mmap_flag = MAP_PRIVATE | MAP_ANONYMOUS;
  if(code != NULL) { mmap_flag |= MAP_FIXED; }

  // 3. Create an area of memory in the monitors address space for the code section
  void* p = 0;
  p = mmap(code + globals_hdr.addr, (globals_pages * page_size), PROT_WRITE, mmap_flag, -1, 0);
  if(p == MAP_FAILED) {
    //perror("mmap");
    //exit(EXIT_FAILURE);
    ps->error = RF_ERR_LASTONE;
    return 0;
  }

  // 4. Read the section body for the globals into a buffer
  //    Put the contents of that buffer into our memory region
  unsigned char* buf = 0;
  if(rf_read_section_body(ps, &globals_hdr, &buf) == 0) {
    memcpy(p, buf, globals_bytes);
  } else {
    //perror("rf_read_section_body");
    //exit(EXIT_FAILURE);
    ps->error = RF_ERR_LASTONE;
    return 0;
  }

  int rc = mprotect(p, (globals_pages * page_size), PROT_WRITE | PROT_READ);
  if(rc < 0) {
    // perror("mprotect");
    // exit(EXIT_FAILURE);
    ps->error = RF_ERR_LASTONE;
    return 0;
  }

  *len = globals_bytes;
  return p;
}

int
rf_exec(const char *path, struct rf_exec_state *exst)
{
  int (*start_fn)();
  // TODO:
  // =====
  //  Implement code that loads and executes a process from a given binary
  //  path.
  struct rf_parse_state ps = {0};
  struct rf_hdr hdr        = {0};
  int err                  = 0;

  // Load the parser state
  if(rf_parse_state_init(&ps, path)) {
    return ps.error;
  }

  // Read and validate the header
  if(rf_read_hdr(&ps, &hdr) || rf_validate_hdr(&hdr, &ps)) {
    err = ps.error;
    rf_parse_state_destroy(&ps);
    return err;
  }

  // 1. Adjust memory so that we respect the locations of the code and globals
  //    required by the CSSE332 RF format.
  int tot_pages = calc_npages(&ps);
  int page_size = getpagesize();
  void* p = 0;
  p = mmap(NULL, (tot_pages * page_size), PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if(p == MAP_FAILED) {
    // perror("mmap");
    // exit(EXIT_FAILURE);
    ps.error = RF_ERR_LASTONE;
    return 0;
  }

  // 2. Load the code segment from the binary.
  int code_bytes = 0;
  void* code_ptr = rf_load_code(&ps, p, &code_bytes);

  // 3. Load the globals segment (if any) from the binary into memory at a
  //    specific location.
  int global_bytes = 0;
  void* globals_ptr = rf_load_data(&ps, p, &global_bytes);
  if(globals_ptr != 0) {
    exst->data = globals_ptr; exst->dlen = global_bytes;
  } else {
    exst->data = 0; exst->dlen=0;
  }
  // Set the exec states
  exst->code = code_ptr; exst->clen = code_bytes;

  // 4. Find the entry point for the newly created virtual process.
  start_fn = (int (*)()) (code_ptr + hdr.entry_offset);
  // 5. Execute the process by simply calling the entry function and capturing
  //    the return value. Save the return value into `exst->rv`.
  int return_value = start_fn();
  exst->rv = return_value;
  // DO NOT unmap the execution state, we do that in rf_unmap_state
  rf_parse_state_destroy(&ps);
  return 0;
}

int
rf_unmap_state(struct rf_exec_state *exst)
{
  int err = 0;
  if(exst->code) {
    err = munmap(exst->code, exst->clen);
    if(err) {
      perror("munmap");
      return -1;
    }
  }
  if(exst->data) {
    err = munmap(exst->data, exst->dlen);
    if(err) {
      perror("munmap");
      return -1;
    }
  }

  return err;
}
