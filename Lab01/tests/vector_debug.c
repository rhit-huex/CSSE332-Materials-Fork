/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

int
main(int argc, char **argv)
{
  struct vector *vec = vec_new();

  // Add any code you'd like to test out independently here. It will be
  // compiled and built into %_debug.run that you can load into gdb.
  printf("TODO: Add vector debug code here if needed...\n");
  printf("TODO: Testing basic push back test case\n");

  //const int answer[] = {10, 20, 30, 40};
  //int i;
  if(!vec) {
    return -1;
  }

  // int i;

  // for(i = 0; i < VEC_DEF_CAP; i++)
  //  vec_push_back(vec, i);

  // should realloc on this push_back
  // vec_push_back(vec, 55);
  // check that the cap updated and length is 5 and the 5th pos (index 4) is 55

  //int i;
  unsigned int el;
  // push and element and then try to remove it.
  vec_push_back(vec, 50);
  el = vec_pop_back(vec);
  //CG_ASSERT_INT_EQ(50, el);
  //CG_ASSERT_INT_EQ(0, vec->len);

  // add two other elements and pop them back.
  vec_push_back(vec, 50);
  vec_push_back(vec, 60);
  el = vec_pop_back(vec);
  printf("%d", el); // so that we don't throw an error
  //CG_ASSERT_INT_EQ(60, el);
  //CG_ASSERT_INT_EQ(1, vec->len);
  el = vec_pop_back(vec);
  //CG_ASSERT_INT_EQ(50, el);
  //CG_ASSERT_INT_EQ(0, vec->len);

  // this push should increase cap
  vec_push_back(vec, 100);
  el = vec_pop_back(vec);
  //CG_ASSERT_INT_EQ(100, el);
  el = vec_pop_back(vec);
  //CG_ASSERT_INT_EQ(590, el);
  //CG_ASSERT_INT_EQ(VEC_DEF_CAP - 1, vec->len);
  //CG_ASSERT_INT_EQ(VEC_DEF_CAP << 1, vec->cap);

  // Check that we didn't overwrite anyting
  vec_free(vec);
  vec = 0;

  return EXIT_SUCCESS;
}
