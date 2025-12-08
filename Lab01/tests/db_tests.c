/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/
#include <stdlib.h>

#include "cgassert.h"
#include "cgproject.h"
#include "cgrade.h"
#include "cgradescropify.h"

#include "db.h"

int
test_node_mknode(void)
{
  struct oo_node n;

  node_mk_node(&n);

  CG_ASSERT_PTR_EQ(n.next, &n);
  CG_ASSERT_PTR_EQ(n.prev, &n);
  return CG_TEST_PASSED;
}

int
test_node_add_tail(void)
{
  struct oo_node head;
  struct oo_node n1, n2, n3;

  node_mk_node(&head);
  node_mk_node(&n1);
  node_mk_node(&n2);
  node_mk_node(&n3);

  node_add_tail(&head, &n1);
  CG_ASSERT_PTR_EQ(head.next, &n1);
  CG_ASSERT_PTR_EQ(head.prev, &n1);
  CG_ASSERT_PTR_EQ(&head, n1.next);
  CG_ASSERT_PTR_EQ(&head, n1.prev);

  node_add_tail(&head, &n2);
  CG_ASSERT_PTR_EQ(&head, n2.next);
  CG_ASSERT_PTR_EQ(&n1, n2.prev);
  CG_ASSERT_PTR_EQ(&n2, n1.next);

  node_add_tail(&head, &n3);
  CG_ASSERT_PTR_EQ(&head, n3.next);
  CG_ASSERT_PTR_EQ(&n2, n3.prev);
  CG_ASSERT_PTR_EQ(&n3, n2.next);

  return CG_TEST_PASSED;
}

int
test_node_add_head(void)
{
  struct oo_node head;
  struct oo_node n1, n2, n3;

  node_mk_node(&head);
  node_mk_node(&n1);
  node_mk_node(&n2);
  node_mk_node(&n3);

  node_add_head(&head, &n1);
  CG_ASSERT_PTR_EQ(head.next, &n1);
  CG_ASSERT_PTR_EQ(head.prev, &n1);
  CG_ASSERT_PTR_EQ(&head, n1.next);
  CG_ASSERT_PTR_EQ(&head, n1.prev);

  node_add_head(&head, &n2);
  CG_ASSERT_PTR_EQ(&n2, head.next);
  CG_ASSERT_PTR_EQ(&head, n2.prev);
  CG_ASSERT_PTR_EQ(&n1, n2.next);
  CG_ASSERT_PTR_EQ(&n2, n1.prev);
  CG_ASSERT_PTR_EQ(&n1, head.prev);

  node_add_head(&head, &n3);
  CG_ASSERT_PTR_EQ(&n3, head.next);
  CG_ASSERT_PTR_EQ(&head, n3.prev);
  CG_ASSERT_PTR_EQ(&n2, n3.next);
  CG_ASSERT_PTR_EQ(&n3, n2.prev);
  CG_ASSERT_PTR_EQ(&n1, head.prev);

  return CG_TEST_PASSED;
}

int
test_node_rm_node(void)
{
  struct oo_node head;
  struct oo_node n1, n2, n3;

  node_mk_node(&head);
  node_mk_node(&n1);
  node_mk_node(&n2);
  node_mk_node(&n3);

  node_add_tail(&head, &n1);
  node_add_tail(&head, &n2);
  node_add_tail(&head, &n3);

  // remove n2 first to expose a gap.
  node_rm_node(&n2);
  CG_ASSERT_PTR_EQ(&n3, n1.next);
  CG_ASSERT_PTR_EQ(&n1, n3.prev);
  CG_ASSERT_PTR_EQ(&head, n3.next);
  CG_ASSERT_PTR_EQ(&head, n1.prev);
  CG_ASSERT_PTR_EQ(&n2, n2.prev);
  CG_ASSERT_PTR_EQ(&n2, n2.next);

  node_rm_node(&n3);
  CG_ASSERT_PTR_EQ(&head, n1.next);
  CG_ASSERT_PTR_EQ(&head, n1.prev);
  CG_ASSERT_PTR_EQ(&n1, head.prev);
  CG_ASSERT_PTR_EQ(&n1, head.prev);

  node_rm_node(&n1);
  CG_ASSERT_PTR_EQ(&head, head.next);
  CG_ASSERT_PTR_EQ(&head, head.prev);

  return CG_TEST_PASSED;
}

int
test_db_string(void)
{
  char *str1 = "hello", *str2 = "world!";
  struct db *db = mk_db("test db strings", DB_STRING);
  struct db_string s1, s2;
  char *r;

  node_mk_node(&s1.node);
  node_mk_node(&s2.node);
  s1.str = str1;
  s2.str = str2;

  db_add_record_type(db, &s1);
  db_add_record_type(db, &s2);
  CG_ASSERT_PTR_EQ(&db->head, s2.node.next);
  CG_ASSERT_PTR_EQ(&db->head, s1.node.prev);
  CG_ASSERT_PTR_EQ(&(s1.node), s2.node.prev);
  CG_ASSERT_PTR_EQ(&(s2.node), s1.node.next);
  CG_ASSERT_PTR_EQ(&(s1.node), db->head.next);
  CG_ASSERT_PTR_EQ(&(s2.node), db->head.prev);

  CG_ASSERT_INT_EQ(2, db->rcount);

  r = join_str_db(db);
  CG_ASSERT_STR_EQ("hello world!", r);

  free(r);
  free(db);
  return CG_TEST_PASSED;
}

int
test_ssd(void)
{
  struct oo_node *n;
  struct db_ulong *ul;
  struct db *db = mk_db("Searching db", DB_ULONG);
  struct db_ulong u[10];
  int even = 0;

  for(int i = 0; i < 10; i++) {
    node_mk_node(&u[i].node);
    u[i].value = i % 2;
    db_add_record_type(db, &u[i]);
  }

  // remove all zeros, so count evens.
  even = searching_seek_and_destroy(db, 0);
  CG_ASSERT_INT_EQ(5, even);
  CG_ASSERT_INT_EQ(5, db->rcount);

  // make sure the remainder of the db is still odd.
  for(n = db->head.next; n != &db->head; n = n->next) {
    ul = node_to_dbulong(n);
    CG_ASSERT_INT_EQ_MSG(1, ul->value,
                         "Make sure all database records are 1s.");
  }

  free(db);
  return CG_TEST_PASSED;
}

struct cg_test_suite *
db_test_suite(void)
{
  struct cg_test_suite *ts =
      cg_test_suite_new("Problem 3: Malleable Linked List Test Suite", 0);

  CG_SUITE_CREATE_GRADED_TEST(ts, "test_node_mknode", test_node_mknode, 0,
                              "Check if the node is initialized correctly.");
  CG_SUITE_CREATE_GRADED_TEST(ts, "test_node_add_tail", test_node_add_tail, 5,
                              "Check if the list adds correctly at the tail");
  CG_SUITE_CREATE_GRADED_TEST(ts, "test_node_add_head", test_node_add_head, 5,
                              "Check if the list adds correctly at the head");
  CG_SUITE_CREATE_GRADED_TEST(ts, "test_node_rm_node", test_node_rm_node, 10,
                              "Check if the list removes a node correctly.");
  CG_SUITE_CREATE_GRADED_TEST(
      ts, "test_db_string", test_db_string, 10,
      "Check that a db of string can be created and joined.");
  CG_SUITE_CREATE_GRADED_TEST(
      ts, "test_seed_and_destroy", test_ssd, 10,
      "Check that removing multiple nodes from the list works well.");

  return ts;
}

int
main(int argc, char **argv)
{
  int rc                  = 0;
  struct cg_project *proj = cg_project_new("Problem 3: Malleable Linked List");
  cg_project_add_suite(proj, db_test_suite());

  cg_project_runall(proj);
  cg_project_summarize(proj);

#ifdef BUILD_GRADESCOPE
  cg_project_gradescopify(proj);
#endif
  rc = proj->num_failures > 0 ? 1 : 0;
  cg_project_del(proj);
  return rc;
}
