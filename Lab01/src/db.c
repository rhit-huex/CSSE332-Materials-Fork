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
#include <string.h>

#include "db.h"

/**
 * Implementation of node_mk_node
 */
void
node_mk_node(struct oo_node *node)
{
  node->next = node->prev = node;
}

/**
 * Implementation of node_rm_node
 */
void
node_rm_node(struct oo_node *node)
{
  // we need to set node->prev->next to be node->next
  //        and set node->next->prev to be node->prev, this removes the node from the middle
  // need a tmp pointer to store the pointer for node->next
  node->next->prev = node->prev;
  node->prev->next = node->next;
  //node->next = NULL;
  //node->prev = NULL; // reset the inner pointers NVM THE TEST CASES DONT WANT THAT
  node->next = node;
  node->prev = node;
}

// This is a helper function, feel free to keep it here or remove it if you do
// not have a need for it.
//static void
//_node_add(struct oo_node *left, struct oo_node *node)
//{
//  left->next->prev = node;
//  node->next       = left->next;
//  node->prev       = left;
//  left->next       = node;
//}

/**
 * Implementation of node_add_tail
 */
void
node_add_tail(struct oo_node *head, struct oo_node *node)
{
  // insert at head->prev since its a circular list

  node->prev = head->prev; // set tail to point at originial tail
  head->prev->next = node; // set originial tail to point to new tail
  node->next = head; // set tail to point back to head

  head->prev = node;
  if (head->next == head) { head->next = node; } // if one element list then the head->next must change
}

/**
 * Implementation of node_add_head
 */
void
node_add_head(struct oo_node *head, struct oo_node *node)
{
  // we essentially want to make a new head
  //actually instructions say insert at the head
  struct oo_node* tmp = head->next;

  node->next = tmp; // set node's next to be rest of the list
  node->prev = head; // node's prev will always become head
  head->next = node; // head's next will always become node

  tmp->prev = node;
  // now we may have to change where head prev points (if it pointed to itself)
  if (head->prev == head) { head->prev = node; }
}

/**
 *Implementation of *node_to_dbstr
 */
struct db_string *
node_to_dbstr(struct oo_node *node)
{
  return (struct db_string *)node;
}

/**
 *Implementation of *node_to_dbulong
 */
struct db_ulong *
node_to_dbulong(struct oo_node *node)
{
  return (struct db_ulong *)node;
}

/**
 * Implementation of db_add_record
 */
void
db_add_record(struct db* db, struct oo_node* node)
{
  // add node at head NVM i think it wanted me to add it at tail
  // node_add_head(&db->head, node);
  node_add_tail(&db->head, node);
  // increase db->rcount by 1
  db->rcount = db->rcount + 1;
}

/**
 * Implementation of mk_db
 */
struct db *
mk_db(const char *name, int rtype)
{
  struct db *db = malloc(sizeof(struct db));
  if(!db) {
    perror("mk_db:");
    exit(EXIT_FAILURE);
  }

  // fail if name is not null terminated.
  memccpy(db->name, name, 0, 256);
  db->rcount  = 0;
  db->db_type = rtype;
  node_mk_node(&db->head);

  return db;
}

/*
 * Implementation of join_str_db
 * */
char *
join_str_db(struct db *db)
{
  struct oo_node *n;
  struct db_string *s;
  char *r = 0, *p = 0;
  size_t len = 0;

  int count = 0;
  for(n = db->head.next; n != &db->head; n = n->next) {
    s = node_to_dbstr(n);
    len += strlen(s->str);
    count++;
  }

  r = malloc(len + count + 1); // allocate space for the total size of all strings plus 1 for the null terminator
  p = r;
  for(n = db->head.next; n != &db->head; n = n->next) {
    s = node_to_dbstr(n);
    strcpy(p, s->str);
    p += strlen(s->str);
    *p++ = ' ';
  }
  *p = 0;

  return r;
}

/*
 * Implementation of remove_all_matching
 */
int
searching_seek_and_destroy(struct db *db, unsigned long value)
{
  struct oo_node* n;
  int removed = 0;

  struct oo_node* tmp_n;
  for(n = db->head.next; n != &db->head; n = tmp_n) {
    // save n->next before we remove n so we can go to next correctly
    tmp_n = n->next;
    struct db_ulong* d = node_to_dbulong(n);
    if(d->value == value) {
      node_rm_node(n);
      removed++;
      db->rcount--;
    }
  }
  return removed;
}
