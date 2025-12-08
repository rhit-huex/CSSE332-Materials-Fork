/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Implementation of the vector data structure for Lab 01 of CSSE332.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <vector.h>

/**
 * Implementation of the vec_new function.
 */
struct vector *
vec_new()
{
  struct vector *vec = malloc(sizeof(struct vector));
  if(!vec) {
    perror("vec:malloc:");
    exit(EXIT_FAILURE);
  }

  vec->len  = 0;
  vec->cap  = VEC_DEF_CAP;
  vec->data = calloc(vec->cap, sizeof(unsigned int));
  if(!vec->data) {
    perror("vec:data:calloc");
    free(vec);
    exit(EXIT_FAILURE);
  }

  return vec;
}

/**
 * Implementation of the vec_free function.
 */
void
vec_free(struct vector *vec)
{
  // TODO: Add your code here...
}

static unsigned int *
__vec_realloc(struct vector *vec)
{
  unsigned int *tmp = vec->data;
  vec->cap          = vec->cap * 2;
  vec->data         = realloc(vec->data, vec->cap * sizeof(unsigned int));
  if(!vec->data) {
    perror("realloc:");
    exit(EXIT_FAILURE);
  }
  return tmp;
}

/**
 * Implementation of the vec_push_back function.
 */
void
vec_push_back(struct vector *vec, unsigned int elem)
{
  // TODO: Add your code here...
}

/**
 * Implementation of the vec_pop_back function.
 */
unsigned int
vec_pop_back(struct vector *vec)
{
  if(!vec || vec->len == 0) {
    return (unsigned int)-1; // doesn't matter what we return.
  }

  // TODO: Add your code here....
  return 0;
}

/**
 * Implementation of the vec_elem_at function..
 */
unsigned int
vec_elem_at(struct vector *vec, unsigned int i)
{
  if(!vec || i >= vec->len) {
    return (unsigned int)-1; // doesn't matter what we return.
  }

  // TODO: Add your code here...
  return 0;
}

/**
 * Implementation of the vec_set_at function..
 */
int
vec_set_at(struct vector *vec, unsigned int i, unsigned int elem)
{
  if(!vec || i >= vec->len) {
    return -1;
  }

  // TODO: Add your code here...
  return 0;
}
