#include "cgproject.h"
#include "cgrade.h"
#include "cgradescropify.h"
#include "fib.h"

int
fib_basic_test(void)
{
  CG_ASSERT_LONG_EQ(0, fib(0));
  CG_ASSERT_LONG_EQ(1, fib(1));

  return CG_TEST_PASSED;
}

int
fib_more_test(void)
{
  int i;
  const unsigned long fibs[] = {
      1,  2,   3,   5,   8,   13,  21,   34,   55,
      89, 144, 233, 377, 610, 987, 1597, 2584, 4181,
  };

  for(i = 0; i < sizeof fibs / sizeof fibs[0]; i++) {
    CG_ASSERT_LONG_EQ_MSG(fibs[i], fib(i + 2), "Check for %ld, expected %lu.",
                          i + 2, fibs[i]);
  }

  return CG_TEST_PASSED;
}

struct cg_test_suite *
fib_test_suite(void)
{
  struct cg_test_suite *ts =
      cg_test_suite_new("Problem 1: Fibonnaci Bug Test Suite", 0);
  CG_SUITE_CREATE_GRADED_TEST(ts, "Test base case", fib_basic_test, 5,
                              "Test the base case for the recursive function.");
  CG_SUITE_CREATE_GRADED_TEST(
      ts, "Test recursive cases", fib_more_test, 15,
      "Test the recursive cases for the recursive function.");
  return ts;
}

int
main(int argc, char **argv)
{
  int rc                  = 0;
  struct cg_project *proj = cg_project_new("Problem 1: Fibonnaci Bug.");
  cg_project_add_suite(proj, fib_test_suite());

  cg_project_runall(proj);
  cg_project_summarize(proj);

#ifdef BUILD_GRADESCOPE
  cg_project_gradescopify(proj);
#endif
  rc = (proj->num_failures > 0) ? 1 : 0;
  cg_project_del(proj);
  return rc;
}
