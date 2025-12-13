/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/
#include <alloca.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cgassert.h"
#include "cgproject.h"
#include "cgrade.h"
#include "cgradescropify.h"

#include "proc_read.h"

volatile unsigned x;

int
test_open_fails(void)
{
  struct program_info pinfo;
  int rc;
  memset(&pinfo, 0, sizeof pinfo);

  rc = open_pmaps_file(&pinfo, 9999999);
  CG_ASSERT_INT_EQ_MSG(-1, rc, "Testing with invalid pid returnd %d.", rc);

  return CG_TEST_PASSED;
}

int
test_open_success(void)
{
  struct program_info pinfo;
  int rc;
  memset(&pinfo, 0, sizeof pinfo);

  rc = open_pmaps_file(&pinfo, getpid());
  CG_ASSERT_INT_EQ_MSG(0, rc, "Testing with valid pid return %d.", rc);

  return CG_TEST_PASSED;
}

int
test_parsing_completes(void)
{
  struct program_info pinfo;
  int rc;
  memset(&pinfo, 0, sizeof pinfo);

  rc = open_pmaps_file(&pinfo, getpid());
  rc = parse_pmaps_file(&pinfo);

  CG_ASSERT_INT_EQ_MSG(
      0, rc, "Check that parsing a valid file returns 0 but returned %d.", rc);
  CG_ASSERT_INT_EQ_MSG(1, pinfo.ready,
                       "Check that parsing marks the pinfo structure as ready");

  return CG_TEST_PASSED;
}

int
test_stack_found_and_ordered(void)
{
  struct program_info pinfo;
  void *start, *end;
  memset(&pinfo, 0, sizeof pinfo);

  open_pmaps_file(&pinfo, getpid());
  parse_pmaps_file(&pinfo);

  start = get_stack_start(&pinfo);
  end   = get_stack_end(&pinfo);
  CG_ASSERT_MSG(start != 0, "Identified stack region start is NULL");
  CG_ASSERT_MSG(end != 0, "Identified stack region end is NULL");
  CG_ASSERT_MSG(start <= end,
                "Stack start and end are not in order end:(%p) < start: (%p).",
                end, start);

  return CG_TEST_PASSED;
}

int
test_stack_contains_sp(void)
{
  struct program_info pinfo;
  void *start, *end;
  void *sp;
  memset(&pinfo, 0, sizeof pinfo);

  open_pmaps_file(&pinfo, getpid());
  parse_pmaps_file(&pinfo);

  start = get_stack_start(&pinfo);
  end   = get_stack_end(&pinfo);
  sp    = alloca(0);
  CG_ASSERT_MSG(
      start <= sp && sp < end,
      "Stack address %p is not contained within your identified stack region.",
      sp);

  return CG_TEST_PASSED;
}

int
test_code_found_and_ordered(void)
{
  struct program_info pinfo;
  void *start, *end;
  memset(&pinfo, 0, sizeof pinfo);

  open_pmaps_file(&pinfo, getpid());
  parse_pmaps_file(&pinfo);

  start = get_code_start(&pinfo);
  end   = get_code_end(&pinfo);
  CG_ASSERT_MSG(start != 0, "Identified code region start is NULL");
  CG_ASSERT_MSG(end != 0, "Identified code region end is NULL");
  CG_ASSERT_MSG(start <= end,
                "Code start and end are not in order end:(%p) < start: (%p).",
                end, start);

  return CG_TEST_PASSED;
}

int
test_code_contains_fn(void)
{
  struct program_info pinfo;
  void *start, *end;
  void *fn;
  memset(&pinfo, 0, sizeof pinfo);

  open_pmaps_file(&pinfo, getpid());
  parse_pmaps_file(&pinfo);

  start = get_code_start(&pinfo);
  end   = get_code_end(&pinfo);
  // NOTE:
  // =====
  //   In here, the code the students write are in a shared library, so they
  //   will not appear here, instead test for the code here.
  fn    = test_code_contains_fn;
  CG_ASSERT_MSG(start <= fn && fn < end,
                "Function address %p is not contained within your identified "
                "code region. start = %p, end = %p",
                fn, start, end);
  fn = test_code_found_and_ordered;
  CG_ASSERT_MSG(start <= fn && fn < end,
                "Function address %p is not contained within your identified "
                "code region. start = %p, end = %p",
                fn, start, end);

  return CG_TEST_PASSED;
}

int
test_heap_found_and_ordered(void)
{
  struct program_info pinfo;
  void *start, *end;
  memset(&pinfo, 0, sizeof pinfo);

  open_pmaps_file(&pinfo, getpid());
  parse_pmaps_file(&pinfo);

  start = get_heap_start(&pinfo);
  end   = get_heap_end(&pinfo);
  CG_ASSERT_MSG(start != 0, "Identified heap region start is NULL");
  CG_ASSERT_MSG(end != 0, "Identified heap region end is NULL");
  CG_ASSERT_MSG(start <= end,
                "heap start and end are not in order end:(%p) < start: (%p).",
                end, start);

  return CG_TEST_PASSED;
}
int
test_heap_contains_malloc(void)
{
  struct program_info pinfo;
  void *start, *end;
  void *p = malloc(1);
  memset(&pinfo, 0, sizeof pinfo);

  open_pmaps_file(&pinfo, getpid());
  parse_pmaps_file(&pinfo);

  start = get_heap_start(&pinfo);
  end   = get_heap_end(&pinfo);
  CG_ASSERT_MSG(start <= p && p < end,
                "Heap allocation %p is not contained within your identified "
                "heap region. start = %p, end = %p",
                p, start, end);

  return CG_TEST_PASSED;
}

int
test_globals_found_and_ordered(void)
{
  struct program_info pinfo;
  void *start, *end;
  memset(&pinfo, 0, sizeof pinfo);

  open_pmaps_file(&pinfo, getpid());
  parse_pmaps_file(&pinfo);

  start = get_globals_start(&pinfo);
  end   = get_globals_end(&pinfo);
  CG_ASSERT_MSG(start != 0, "Identified globals region start is NULL");
  CG_ASSERT_MSG(end != 0, "Identified globals region end is NULL");
  CG_ASSERT_MSG(
      start <= end,
      "globals start and end are not in order end:(%p) < start: (%p).", end,
      start);

  return CG_TEST_PASSED;
}

int
test_globals_contains_var(void)
{
  struct program_info pinfo;
  void *start, *end;
  x = 3;
  memset(&pinfo, 0, sizeof pinfo);

  open_pmaps_file(&pinfo, getpid());
  parse_pmaps_file(&pinfo);

  start = get_globals_start(&pinfo);
  end   = get_globals_end(&pinfo);
  CG_ASSERT_MSG(start <= (void *)&x && (void *)&x < end,
                "global variable %p is not contained within your identified "
                "globals region. start = %p, end = %p",
                &x, start, end);

  return CG_TEST_PASSED;
}

struct cg_test_suite *
proc_read_test_suite(void)
{
  struct cg_test_suite *ts =
      cg_test_suite_new("Homework 02: Conding Challenge", 0);

  CG_SUITE_CREATE_GRADED_TEST(ts, "test_open_fails", test_open_fails, 5,
                              "Check if open fails on invalid pid");
  CG_SUITE_CREATE_GRADED_TEST(
      ts, "test_open_success", test_open_success, 5,
      "Check if open returns success with current process pid");
  CG_SUITE_CREATE_GRADED_TEST(ts, "test_stack_found_and_ordered",
                              test_stack_found_and_ordered, 10,
                              "Check if stack addresses found are oredered");
  CG_SUITE_CREATE_GRADED_TEST(
      ts, "test_stack_contains_sp", test_stack_contains_sp, 10,
      "Check if stack allocation is within identified stack region");
  CG_SUITE_CREATE_GRADED_TEST(ts, "test_code_found_and_ordered",
                              test_code_found_and_ordered, 5,
                              "Check if code addresses found are ordered");
  CG_SUITE_CREATE_GRADED_TEST(
      ts, "test_code_contains_fn", test_code_contains_fn, 10,
      "Check if function address is within identified region");
  CG_SUITE_CREATE_GRADED_TEST(ts, "test_heap_found_and_ordered",
                              test_heap_found_and_ordered, 5,
                              "Check if heap addresses found are ordered");
  CG_SUITE_CREATE_GRADED_TEST(
      ts, "test_heap_contains_malloc", test_heap_contains_malloc, 10,
      "Check if heap allocation using malloc is within identified region");
  CG_SUITE_CREATE_GRADED_TEST(
      ts, "test_globals_found_and_ordered", test_globals_found_and_ordered, 5,
      "Check if globals region addresses are found and ordered");
  CG_SUITE_CREATE_GRADED_TEST(
      ts, "test_globals_contains_var", test_globals_contains_var, 10,
      "Check if globals regions contains global variable");

  return ts;
}

int
main(int argc, char **argv)
{
#ifdef BUILD_GRADESCOPE
  struct cg_test_suite *ts = proc_read_test_suite();
  int rc                   = cg_test_suite_runall(ts);

  cg_test_suite_summarize(ts);
  cg_test_suite_gradescopify_tests(ts, "proc_read_tests.run.json");

  cg_test_suite_remove(ts);
  return (rc > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
#else
  int rc                  = 0;
  struct cg_project *proj = cg_project_new("Homework 02: Parsing procfs");
  cg_project_add_suite(proj, proc_read_test_suite());

  cg_project_runall(proj);
  cg_project_summarize(proj);

  rc = proj->num_failures > 0 ? 1 : 0;
  cg_project_del(proj);
  return rc;
#endif
}
