/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */

#include <errno.h>
#include <sys/mman.h>

#include "rhmalloc.h"

/**
 * For testing purposes, we need to record where our memory starts. Generally
 * this is hidden from the users of the library but we're just using it here to
 * make our tests more meaningful.
 */
static void *heap_mem_start = 0;

/**
 * Head of the free list. It is actually a pointer to the header of the first
 * free block.
 *
 * @warning
 *  In this assignment, "freelist" is somewhat of a misnomer, because
 *  this list contains both free and unfree nodes.
 */
static struct metadata *freelist = 0;

struct metadata *
freelist_head(void)
{
  return freelist;
}

void *
heap_start(void)
{
  return heap_mem_start;
}

int
rhmalloc_init(void)
{
  void *p = 0;

  p = mmap(NULL, MAX_HEAP_SIZE, PROT_READ | PROT_WRITE,
           MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if(p == MAP_FAILED) {
    errno = ENOMEM;
    return -1;
  }

  // TODO
  // =====
  //  Add code here to initialize heap_mem_start, freelist, and the content of
  //  freelist.
  return 0;
}

int
rhfree_all(void)
{
  int rc         = munmap(heap_mem_start, MAX_HEAP_SIZE);
  heap_mem_start = 0;
  freelist       = 0;
  return rc;
}

void *
rhmalloc(size_t size)
{
  // check if we need to reset the space.
  if(!freelist && rhmalloc_init())
    return 0;

  // align the size
  size = ALIGN(size);

  // TODO:
  // =====
  //  Add code here to find a suitable block and return a pointer to the start
  //  of the usable memory region for it.

  // return here when we can't find a block, so set errno to ENOMEM.
  errno = ENOMEM;
  return 0;
}

void
rhfree(void *p)
{
  // TODO:
  // =====
  //  Add code here to coalese the block to free with the next and previous
  //  blocks if applicable.
}
