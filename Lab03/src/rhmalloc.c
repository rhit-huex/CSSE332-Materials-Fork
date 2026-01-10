/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * @author Evan Hu
 * @date
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
  heap_mem_start = p; // Allocated space starts at where mmap returned
  freelist = heap_mem_start; // We are only storing the LL in our allocated space
  // Need to create the free block
  // Create a metadata instance and give it size=MAX_HEAP_SIZE, in_use=1, next and prev can be null
  // I think the data will come right after the metadata so if I think about it linear
  // The next pointer must point past all the space for data
  // and the prev pointer would have to point past all the previous space for data

  // Initialize the head's metadata
  freelist->size = MAX_HEAP_SIZE - sizeof(struct metadata);
  freelist->in_use = 0;
  freelist->prev = NULL;
  freelist->next = NULL;
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

  // 1. Traverse the LL starting at freelist.
  // 2. Stop on either of these cases:
  //    a. We find a block that is not in use and has enough space (use the existing metadata)
  //          Simply return the pointer to the data from this metadata
  //    b. We don't find a block and reach the free block (last block) (create new metadata)
  //       We will have to check if we are at the end of the list so curNode->next != NULL
  //       If the last block's prev ptr is NULL (empty list):
  //          Create a new metadata and set its prev to NULL and its next to curNode
  //          Set curNode's prev ptr to the new metadata location
  //       If the free block doesn't have enough space, break and error

  // For actually creating the new nodes while account for space for the user data:
  // We first need to get the address of the block. So take the last block's address then we can
  // create the block right there and then we can reassign where the last block points
  // return here when we can't find a block, so set errno to ENOMEM.

  // For calculating where to start the next metadata start from the current metadata pointer add 
  // size of data and metadata and then assign that location to the pointers in the current 
  // metadata

  struct metadata* curNode = freelist;
  int block_found = 0;
  while(curNode != NULL) {
    if(curNode->in_use == 1) { curNode = curNode->next; continue; }
    if(curNode->size == size) { // case where the space is exactly the size of what we need (no splitting)
      curNode->in_use = 1;
      return (char*)curNode + sizeof(struct metadata);
    }
    // newSize is the left over size after we would allocate the new node, we will set the free blocks size to this
    int newSize = curNode->size - (sizeof(struct metadata) + size);
    if(newSize > 0) {
      // We found a block we can split, now split it and break from the loop
      // Have the current metadata ptr (curNode) be the allocated space then make a new struct that comes after
      // that will be the remaining free space block
      struct metadata* splitNode = (struct metadata*)((char*)curNode + sizeof(struct metadata) + size);
      splitNode->next = curNode->next;
      if(curNode->next != NULL) { curNode->next->prev = splitNode; }
      splitNode->prev = curNode;
      splitNode->in_use = 0;
      splitNode->size = newSize;

      // Now we set the reset the pointers for existing metadata (prev stays same)
      curNode->next = splitNode;
      curNode->size = size;
      curNode->in_use = 1;
      // prev and can stay the same

      block_found = 1;
      return (char*)curNode + sizeof(struct metadata); // Return pointer to allocated user data block
    }
    curNode = curNode->next; // advance the loop if we didn't allocate
  }
  if(block_found == 0) { errno = ENOMEM; return 0;}
  return 0;
}

void
rhfree(void *p)
{
  // TODO:
  // =====
  //  Add code here to coalese the block to free with the next and previous
  //  blocks if applicable.

  // First free the current block (we always do this)
  struct metadata* meta_ptr = (struct metadata*)((char*)p - sizeof(struct metadata));
  meta_ptr->in_use = 0;
  // Left coalesce:
  struct metadata* left_node = meta_ptr->prev; // left node (may be null at this point)
  struct metadata* right_node = meta_ptr->next; //get the right node (may also be null at this point)

  int can_merge_left = (left_node != NULL) && (left_node->in_use == 0);
  int can_merge_right = (right_node != NULL) && (right_node->in_use == 0);

  if(can_merge_left && can_merge_right) {
    // merge both
    // Keep the leftmost meta data intact
    // Change left node's next to point to where right_node is point to next
    // Change the left node's size to be cursize + 8 + size of curnode + 8 + size of right node
    left_node->size = left_node->size + (2 * sizeof(struct metadata)) + meta_ptr->size + right_node->size;

    if(right_node->next != NULL) { right_node->next->prev = left_node; }
    left_node->next = right_node->next;
  } else if(can_merge_left) {
    // merge left only
    left_node->size = left_node->size + sizeof(struct metadata) + meta_ptr->size;

    if(right_node != NULL) { right_node->prev = left_node; }
    left_node->next = right_node;
  } else if(can_merge_right) {
    // merge right only
    meta_ptr->size = meta_ptr->size + sizeof(struct metadata) + right_node->size;

    if(right_node->next != NULL) { right_node->next->prev = meta_ptr; }
    meta_ptr->next = right_node->next;
  } else {
    // no merge
  }
}
