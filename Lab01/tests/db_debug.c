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

#include "db.h"

int
main(int argc, char **argv)
{
  // Add any code you'd like to test out independently here. It will be
  // compiled and built into %_debug.run that you can load into gdb.
  printf("TODO: Add db debug code here if needed...\n");

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
  //CG_ASSERT_PTR_EQ(&db->head, s2.node.next);
  //CG_ASSERT_PTR_EQ(&db->head, s1.node.prev);
  //CG_ASSERT_PTR_EQ(&(s1.node), s2.node.prev);
  //CG_ASSERT_PTR_EQ(&(s2.node), s1.node.next);
  //CG_ASSERT_PTR_EQ(&(s1.node), db->head.next);
  //CG_ASSERT_PTR_EQ(&(s2.node), db->head.prev);

  //CG_ASSERT_INT_EQ(2, db->rcount);

  r = join_str_db(db);
  //CG_ASSERT_STR_EQ("hello world!", r);

  free(r);
  free(db);

  return EXIT_SUCCESS;
}
