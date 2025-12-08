/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/
#include "cgassert.h"
#include "cgrade.h"
#include "cgradescropify.h"
#include "vector.h"

int
test_vec_new(void)
{
  struct vector *vec = vec_new();

  CG_ASSERT_PTR_NNUL(vec);
  CG_ASSERT_INT_EQ(0, vec->len);
  CG_ASSERT_INT_EQ(VEC_DEF_CAP, vec->cap);
  CG_ASSERT_PTR_NNUL(vec->data);

  // For now, there's no way to test if this free'ing everything, so we will
  // delay that and run valgrind at the end.
  vec_free(vec);
  return CG_TEST_PASSED;
}

int
test_vec_basic_push_back(void)
{
  const int answer[] = {10, 20, 30, 40};
  int i;
  struct vector *vec = vec_new();
  if(!vec) {
    return CG_TEST_FAILED;
  }

  vec_push_back(vec, 10);
  CG_ASSERT_INT_EQ(10, vec->data[0]);
  CG_ASSERT_INT_EQ(1, vec->len);
  vec_push_back(vec, 20);
  CG_ASSERT_INT_EQ(20, vec->data[1]);
  CG_ASSERT_INT_EQ(2, vec->len);
  vec_push_back(vec, 30);
  CG_ASSERT_INT_EQ(30, vec->data[2]);
  CG_ASSERT_INT_EQ(3, vec->len);
  vec_push_back(vec, 40);
  CG_ASSERT_INT_EQ(40, vec->data[3]);
  CG_ASSERT_INT_EQ(4, vec->len);

  // check the cap is still the same
  CG_ASSERT_INT_EQ(VEC_DEF_CAP, vec->cap);

  // Check that we didn't overwrite anyting
  for(i = 0; i < 4; i++) {
    CG_ASSERT_INT_EQ(answer[i], vec->data[i]);
  }

  vec_free(vec);
  vec = 0;
  return CG_TEST_PASSED;
}

int
test_vec_push_back_realloc(void)
{
  int i;
  struct vector *vec = vec_new();
  if(!vec)
    return CG_TEST_FAILED;

  for(i = 0; i < VEC_DEF_CAP; i++)
    vec_push_back(vec, i);

  // should realloc on this push_back
  vec_push_back(vec, 5);
  CG_ASSERT_INT_EQ(8, vec->cap);
  CG_ASSERT_INT_EQ(5, vec->len);
  CG_ASSERT_INT_EQ(5, vec->data[4]);

  for(i = 0; i < VEC_DEF_CAP; i++) {
    CG_ASSERT_INT_EQ_MSG(
        i, vec->data[i],
        "Checking if data is maintained after expansion, iteration %d.", i);
  }

  vec_free(vec);
  vec = 0;
  return CG_TEST_PASSED;
}

int
test_vec_pop_back(void)
{
  int i;
  unsigned int el;
  struct vector *vec = vec_new();
  if(!vec)
    return CG_TEST_FAILED;

  // push and element and then try to remove it.
  vec_push_back(vec, 50);
  el = vec_pop_back(vec);
  CG_ASSERT_INT_EQ(50, el);
  CG_ASSERT_INT_EQ(0, vec->len);

  // add two other elements and pop them back.
  vec_push_back(vec, 50);
  vec_push_back(vec, 60);
  el = vec_pop_back(vec);
  CG_ASSERT_INT_EQ(60, el);
  CG_ASSERT_INT_EQ(1, vec->len);
  el = vec_pop_back(vec);
  CG_ASSERT_INT_EQ(50, el);
  CG_ASSERT_INT_EQ(0, vec->len);

  // make sure capcity does not change too early
  for(i = 0; i < VEC_DEF_CAP; i++)
    vec_push_back(vec, 590);

  CG_ASSERT_INT_EQ(VEC_DEF_CAP, vec->len);
  CG_ASSERT_INT_EQ(VEC_DEF_CAP, vec->cap);

  // this push should increase cap
  vec_push_back(vec, 100);
  el = vec_pop_back(vec);
  CG_ASSERT_INT_EQ(100, el);
  el = vec_pop_back(vec);
  CG_ASSERT_INT_EQ(590, el);
  CG_ASSERT_INT_EQ(VEC_DEF_CAP - 1, vec->len);
  CG_ASSERT_INT_EQ(VEC_DEF_CAP << 1, vec->cap);

  vec_free(vec);
  return CG_TEST_PASSED;
}

int
test_vec_elem_at(void)
{
  struct vector *vec = vec_new();
  if(!vec)
    return CG_TEST_FAILED;

  vec_push_back(vec, 100);
  CG_ASSERT_INT_EQ(100, vec_elem_at(vec, 0));
  CG_ASSERT_INT_EQ(1, vec->len);

  vec_push_back(vec, 200);
  CG_ASSERT_INT_EQ(200, vec_elem_at(vec, 1));
  CG_ASSERT_INT_EQ(100, vec_elem_at(vec, 0));
  CG_ASSERT_INT_EQ(2, vec->len);

  // check for edge cases
  CG_ASSERT_INT_EQ((unsigned int)-1, vec_elem_at(0, 0));
  CG_ASSERT_INT_EQ((unsigned int)-1, vec_elem_at(vec, vec->len));

  vec_free(vec);
  return CG_TEST_PASSED;
}

int
test_vec_set_at(void)
{
  struct vector *vec = vec_new();
  if(!vec)
    return CG_TEST_FAILED;

  // check error case first
  CG_ASSERT_INT_EQ((unsigned int)-1, vec_set_at(0, 0, 0));
  CG_ASSERT_INT_EQ((unsigned int)-1, vec_set_at(vec, 0, 0));

  // check update takes place.
  vec_push_back(vec, 100);
  CG_ASSERT_INT_EQ(0, vec_set_at(vec, 0, 500));
  CG_ASSERT_INT_EQ(500, vec_elem_at(vec, 0));

  // check other than 0. vec_push_back(vec, 30);
  vec_push_back(vec, 40);
  CG_ASSERT_INT_EQ(40, vec_elem_at(vec, vec->len - 1));
  CG_ASSERT_INT_EQ(0, vec_set_at(vec, vec->len - 1, 3));
  CG_ASSERT_INT_EQ(3, vec_elem_at(vec, vec->len - 1));

  vec_free(vec);
  return CG_TEST_PASSED;
}

struct cg_test_suite *
vector_test_suite(void)
{
  struct cg_test_suite *ts =
      cg_test_suite_new("Problem 2: Vector Test Suite ", 0);

  CG_SUITE_CREATE_GRADED_TEST(
      ts, "Test vector allocation", test_vec_new, 5,
      "This test will check that vec_new will generate correct values.");

  CG_SUITE_CREATE_GRADED_TEST(
      ts, "Test basic vector push back", test_vec_basic_push_back, 10,
      "This test will make sure that basic push back works, it "
      "involves no need for resizing, that is a separate test");

  CG_SUITE_CREATE_GRADED_TEST(
      ts, "Test vector resize", test_vec_push_back_realloc, 10,
      "This test will check that an increase in the capacity of "
      "the vector once enough elements have been pushed back.");

  CG_SUITE_CREATE_GRADED_TEST(
      ts, "Test vector pop back", test_vec_pop_back, 10,
      "This test will check that removing elements from the vector "
      "works correctly, affects length, and does not affect capacity.");

  CG_SUITE_CREATE_GRADED_TEST(
      ts, "Test vector element at", test_vec_elem_at, 5,
      "This test will check that accessing elements works correctly and "
      "has proper error handling.");

  CG_SUITE_CREATE_GRADED_TEST(
      ts, "Test vector set at", test_vec_set_at, 5,
      "This test will check that modifying elements at a specific index "
      "works and performs error handling.");

  return ts;
}

int
main(int argc, char **argv)
{
  int rc                  = 0;
  struct cg_project *proj = cg_project_new("Problem 2: Vector");
  cg_project_add_suite(proj, vector_test_suite());

  cg_project_runall(proj);
  cg_project_summarize(proj);

#ifdef BUILD_GRADESCOPE
  cg_project_gradescopify(proj);
#endif
  rc = proj->num_failures > 0 ? 1 : 0;
  cg_project_del(proj);
  return rc;
}
