/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "cgassert.h"
#include "cgrade.h"
#include "ilist.h"

int
test_insert_at_end(void)
{
  struct ilist_node n1 = {.next = 0, .value = 5};
  struct ilist_node n2 = {.next = 0, .value = 10};
  struct ilist_node n3 = {.next = 0, .value = 15};
  struct ilist_node n4 = {.next = 0, .value = 20};

  insert_at_end(&n1, &n2);
  CG_ASSERT_PTR_EQ_MSG(&n2, n1.next,
                       "Insertion does not modify the next field.");
  insert_at_end(&n1, &n3);
  CG_ASSERT_PTR_EQ_MSG(&n3, n2.next,
                       "Insertion does not happen at the tail of the list.");
  insert_at_end(&n2, &n4);
  CG_ASSERT_PTR_EQ_MSG(&n4, n3.next,
                       "Insertion at tail is sensitive to where the head is.");
  insert_at_end(&n1, &n1);
  CG_ASSERT_MSG(n1.next != &n1, "Your list insertion allows for self loops.");

  return CG_TEST_PASSED;
}

int
test_find_node(void)
{
  struct ilist_node n4 = {.next = 0, .value = 20};
  struct ilist_node n3 = {.next = &n4, .value = 15};
  struct ilist_node n2 = {.next = &n3, .value = 10};
  struct ilist_node n1 = {.next = &n2, .value = 5};

  CG_ASSERT_PTR_EQ_MSG(&n1, find_node(&n1, 5),
                       "Attempt to find node with value 5 failed.");
  CG_ASSERT_PTR_EQ_MSG(&n2, find_node(&n1, 10),
                       "Attempt to find node with value 10 failed.");
  CG_ASSERT_PTR_EQ_MSG(&n3, find_node(&n1, 15),
                       "Attempt to find node with value 15 failed.");
  CG_ASSERT_PTR_EQ_MSG(&n4, find_node(&n1, 20),
                       "Attempt to find node with value 20 failed.");
  CG_ASSERT_PTR_EQ_MSG(
      0, find_node(&n1, 0),
      "Note with value 0 found while it should not have been.");

  return CG_TEST_PASSED;
}

int
test_get_node_count(void)
{
  struct ilist_node nodes[128];
  int i = 0;

  for(i = 0; i < 128; i++) {
    nodes[i].next  = 0;
    nodes[i].value = i;
    insert_at_end(&nodes[0], &nodes[i]);
    CG_ASSERT_INT_EQ_MSG(
        i + 1, get_node_count(&nodes[0]),
        "After %d insertions, your list did not have %d nodes.", i + 1, i + 1);
  }

  return CG_TEST_PASSED;
}

int
test_to_array(void)
{
  struct ilist_node nodes[128];
  int array[128];
  int *p;
  int i = 0;

  for(i = 0; i < 128; i++) {
    nodes[i].next  = 0;
    nodes[i].value = i;
    array[i]       = i;
    insert_at_end(&nodes[0], &nodes[i]);
  }
  p = ilist_to_array(&nodes[0]);
  CG_ASSERT(!memcmp(array, p, 128 * sizeof(int)));
  free(p);
  return CG_TEST_PASSED;
}

int
test_delete_vals(void)
{
  struct ilist_node n4 = {.next = 0, .value = 20};
  struct ilist_node n3 = {.next = &n4, .value = 15};
  struct ilist_node n2 = {.next = &n3, .value = 10};
  struct ilist_node n1 = {.next = &n2, .value = 5};

  delete_vals(&n1, 20, 0);
  CG_ASSERT_PTR_EQ_MSG(0, n3.next, "Removing 20 did not delete n4.");
  CG_ASSERT_PTR_EQ_MSG(&n3, n2.next, "Removing 20 modified the wrong nodes.");
  CG_ASSERT_PTR_EQ_MSG(&n2, n1.next, "Removing 20 modified the wrong nodes.");

  delete_vals(&n1, 10, 0);
  CG_ASSERT_PTR_EQ_MSG(&n3, n1.next, "Removing 10 did not delete n2.");
  CG_ASSERT_PTR_EQ_MSG(0, n3.next, "Removing 10 modified the wrong nodes.");

  delete_vals(&n1, 15, 0);
  CG_ASSERT_PTR_EQ_MSG(0, n1.next,
                       "Removing 15 did not make n1 the only node.");

  // test removing multiple nodes
  // [10, 20, 20, 10]
  n1.value = 10;
  n2.value = 20;
  n3.value = 20;
  n4.value = 20;
  n1.next  = &n2;
  n2.next  = &n3;
  n3.next  = &n4;

  delete_vals(&n1, 20, 0);
  CG_ASSERT_PTR_EQ_MSG(0, n1.next,
                       "Removing duplicates does not catch all of them.");

  return CG_TEST_PASSED;
}

struct cg_test_suite *
ilist_test_suite(void)
{
  struct cg_test_suite *ts =
      cg_test_suite_new("Linked list implementation demo", 0);

  CG_SUITE_CREATE_GRADED_TEST(
      ts, "test_insert_at_end", test_insert_at_end, 10,
      "Test insertion at the end of list, regardless where we start.");
  CG_SUITE_CREATE_GRADED_TEST(ts, "test_find_node", test_find_node, 10,
                              "Test that search finds the first node that "
                              "matches the value in the list.");
  CG_SUITE_CREATE_GRADED_TEST(
      ts, "test_get_node_count", test_get_node_count, 5,
      "Test the counting the number of nodes in the list is correct.");
  CG_SUITE_CREATE_GRADED_TEST(
      ts, "test_to_array", test_to_array, 10,
      "Test that array representation of list is built correctly.");
  CG_SUITE_CREATE_GRADED_TEST(ts, "test_delete_vals", test_delete_vals, 15,
                              "Test that deletion from the list works with "
                              "different numbers and duplicates.");
  return ts;
}

int
main(int argc, char **argv)
{
  int rc                   = 0;
  struct cg_test_suite *ts = ilist_test_suite();

  cg_test_suite_runall(ts);
  cg_test_suite_summarize(ts);

  rc = ts->num_failures > 0 ? 1 : 0;
  cg_test_suite_remove(ts);
  return rc;
}
