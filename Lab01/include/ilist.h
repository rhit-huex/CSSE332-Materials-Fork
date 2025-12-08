/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/
#ifndef _ILIST_H
#define _ILIST_H

/**
 * A node in singly linked list of integers.
 */
struct ilist_node {
  int value;               /**< The value in the node */
  struct ilist_node *next; /**< Pointer to the next iterm in the list */
};

/**
 * Insert a new ilist node at the tail of the current list.
 *
 * @param head  The head or any entry in the list to insert at.
 * @param n     The entry to add to the list.
 */
void insert_at_end(struct ilist_node *head, struct ilist_node *n);

/**
 * Search for a given value in the list represented by head.
 *
 * @param head  The node to start the search from.
 * @param val   The value to search for.
 *
 * @return The first node containing the value val, 0 otherwise.
 */
struct ilist_node *find_node(struct ilist_node *head, int val);

/**
 * Get the number of nodes in the list starting at head.
 *
 * @param head  The node to start counting from.
 *
 * @return The number of nodes in the list starting at head.
 */
int get_node_count(struct ilist_node *head);

/**
 * Delete ALL the nodes that contain the value val from the list starting at
 * head.
 *
 * @param head    The starting node of the list to search into.
 * @param val     The value to search for.
 * @param dofree  Flag to indicate whether the nodes should be free'd
 */
void delete_vals(struct ilist_node *head, int val, int dofree);

/**
 * Transform the linked list starting at head into an indexed array.
 *
 * This function should allocate the room for the array so that it can return
 * it. it is the responsibility of the user to free up the space afterwads.
 *
 * @param head  The starting node of the list to transform.
 *
 * @return an allocated array of integers with the same values as the list.
 */
int *ilist_to_array(struct ilist_node *head);

#endif /* ilist.h */
