/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/

#include <string.h>

#include "cgassert.h"
#include "cgrade.h"
#include "cgradescropify.h"
#include "mem.h"

static int
test_createmem(void)
{
  int nc    = 3;
  int ni    = 2;
  void *mem = 0;

  // create a slab of memory.
  mem = getmem(nc, ni);
  CG_ASSERT_INT_EQ(nc, getnc(mem));
  CG_ASSERT_INT_EQ(ni, getni(mem));
  // done with this one
  freemem(mem);

  // try again with different numbers
  nc = 0, ni = 5;

  mem = getmem(nc, ni);
  CG_ASSERT_INT_EQ(nc, getnc(mem));
  CG_ASSERT_INT_EQ(ni, getni(mem));
  freemem(mem);

  // one last time
  nc = 5, ni = 0;

  mem = getmem(nc, ni);
  CG_ASSERT_INT_EQ(nc, getnc(mem));
  CG_ASSERT_INT_EQ(ni, getni(mem));
  freemem(mem);

  return CG_TEST_PASSED;
}

static int
test_getstr(void)
{
  // create only a bunch of characters.
  int nc = 10;
  void *mem;
  char *p;

  mem = getmem(nc, 0);
  p   = getstr(mem);

  CG_ASSERT_INT_EQ(nc, getnc(mem));
  CG_ASSERT_INT_EQ(0, getni(mem));

  // silly test here but just a quick check that we don't seg fault.
  CG_ASSERT('\0' == *getstr(mem));
  *p = 'a';
  CG_ASSERT('a' == *getstr(mem));

  freemem(mem);
  return CG_TEST_PASSED;
}

static int
test_cpstr(void)
{
  // This function will test whether the cpstr function works correctly in two
  // ways:
  // 1. Does the string get copied if it was shorter than the size of the
  //    memory area.
  // 2. Is the string trimmed and a null byte inserted if it was longer than
  //    the content of the memory area.
  //

  const char *msg        = "Hello World!";
  const char *longer_msg = "Hello World! Hello World!";
  void *mem              = 0;
  size_t nc              = strlen(msg) + 2;
  size_t len             = 0;
  char *p                = 0;

  mem = getmem(nc, 0);
  p   = getstr(mem);

  // check on the len value
  len = cpstr(mem, msg, strlen(msg));
  CG_ASSERT_INT_EQ_MSG(
      strlen(msg) + 1, len,
      "Check that return value includes the null termiantor inserted.");
  CG_ASSERT_STR_EQ_MSG(
      msg, p, "Check that the string is indeed copied over to the memory area");

  // grab a longer string.
  len = cpstr(mem, longer_msg, strlen(longer_msg));
  CG_ASSERT_INT_EQ_MSG(nc, len,
                       "Check that the return value is upper bound by nc.");
  CG_ASSERT_STR_EQ_MSG("Hello World! ", p,
                       "Check that the string itself is trimmed down to the "
                       "available number of bytes.");

  freemem(mem);
  return CG_TEST_PASSED;
}

static int
test_getint(void)
{
  // Test the getintptr, getint_at, and setint_at family of functions.
  void *mem         = 0;
  const int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  size_t len        = sizeof(array) / sizeof(array[0]);
  int *p            = 0;
  int i, res;

  mem = getmem(0, len);
  p   = getintptr(mem);

  // make sure we return a non-null pointer.
  CG_ASSERT_PTR_NNUL(p);

  // fill in the values.
  memcpy(p, array, len * sizeof(int));
  CG_ASSERT_MSG(!memcmp(array, getintptr(mem), len * sizeof(int)),
                "Check that copying data over into the memory area actually "
                "results in updates to that area.");

  // test the setters and getters
  for(i = 0; i < len; i++) {
    getint_at(mem, i, &res);
    CG_ASSERT_INT_EQ_MSG(array[i], res,
                         "Check that get_element_at %d returns the right value",
                         i);
  }

  // check that getter should fail for incorrect index.
  res = getint_at(mem, len, 0);
  CG_ASSERT_INT_EQ_MSG(-1, res,
                       "Check that invalid index is reported as error.");

  // test setters
  res = setint_at(mem, 2, 100);
  CG_ASSERT_INT_EQ_MSG(100, p[2],
                       "Check that changing the value at index 2 takes effect");
  CG_ASSERT_INT_EQ_MSG(0, res, "Check that setint_at returns 0 on success.")

  // test setter with invalid
  res = setint_at(mem, len, 500);
  CG_ASSERT_INT_EQ_MSG(-1, res,
                       "Check that setint_at returns -1 on invalid index");

  freemem(mem);
  return CG_TEST_PASSED;
}

struct cg_test_suite *
mem_test_suite(void)
{
  struct cg_test_suite *ts =
      cg_test_suite_new("Homework Problem: Punned Memory", 0);

  CG_SUITE_CREATE_GRADED_TEST(
      ts, "test_createmem", test_createmem, 10,
      "Test the creation of memory slabs and that we can recover sizes.");
  CG_SUITE_CREATE_GRADED_TEST(
      ts, "test_getstr", test_getstr, 5,
      "Test whether we can reach the string part of the slab.");
  CG_SUITE_CREATE_GRADED_TEST(ts, "test_cpstr", test_cpstr, 10,
                              "Test whether the string copy function behaves "
                              "according to specifications");
  CG_SUITE_CREATE_GRADED_TEST(
      ts, "test_getint", test_getint, 15,
      "Test the behavior of getintptr, getint_at, and setint_at");

  return ts;
}

int
main(int argc, char **argv)
{
  int rc                  = 0;
  struct cg_project *proj = cg_project_new("Homework Problem: Punned Memory");

  cg_project_add_suite(proj, mem_test_suite());

  cg_project_runall(proj);
  cg_project_summarize(proj);

#ifdef BUILD_GRADESCOPE
  cg_project_gradescopify(proj);
#endif
  rc = proj->num_failures > 0 ? 1 : 0;
  cg_project_del(proj);
  return rc;
}
