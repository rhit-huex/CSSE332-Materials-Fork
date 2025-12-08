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

#include "ilist.h"

int
main(int argc, char **argv)
{
  struct ilist_node head, n1, n2, n3;
  int nc;

  head.next = 0;
  n1.next   = 0;
  n2.next   = 0;
  n3.next   = 0;

  insert_at_end(&head, &n1);
  insert_at_end(&head, &n2);
  insert_at_end(&head, &n3);

  nc = get_node_count(&head);
  printf(" The number of nodes from get_node_count is %d!", nc);

  return EXIT_SUCCESS;
}
