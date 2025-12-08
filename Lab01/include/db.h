/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/
#ifndef _DB_H
#define _DB_H

/**
 * An malleable node abstraction.
 *
 * This data structure approach is based on the list_node approach found in the
 * Linux kernel.
 */
struct oo_node {
  struct oo_node *prev; /**< The previous neighbor */
  struct oo_node *next; /**< The next neighbor */
};

enum rtype {
  DB_STRING = 0, /**< A database of strings */
  DB_ULONG,      /**< A database of unsigned longs */
  DB_LASTONE     /**< Leave this as the last one */
};

struct db {
  char name[256];     /**< The name of the database */
  int rcount;         /**< The record count */
  enum rtype db_type; /** The type of the records in the databse. */

  struct oo_node head; /**< The head of the list of records */
};

/**
 * A database record holding a string.
 */
struct db_string {
  struct oo_node node; /**< This must be the first element ALL THE TIME */
  char *str;           /**< A pointer to the actual string */
};

/**
 * A database record holding an unsigned integer.
 */
struct db_ulong {
  struct oo_node node; /**< This must be the first element ALL THE TIME */
  unsigned long value; /**< The value in the databse entry */
};

/************************************************************************************
 * Abstract list node functions.
 ***********************************************************************************/
/**
 * Initialize an oo abstract node.
 *
 * @param node  The abstract node to initialize.
 */
void node_mk_node(struct oo_node *node);

/**
 * Remove an abstract node from its list.
 *
 * @param node  The abstract node to initialize.
 */
void node_rm_node(struct oo_node *node);

/**
 * Add a node to the tail of a list.
 *
 * @warning
 *  You need not to loop in this case.
 *
 * @param head  The sentinel head of the list.
 * @param node  The abstract node to add.
 */
void node_add_tail(struct oo_node *head, struct oo_node *node);

/**
 * Add a node at the head of a list.
 *
 * @warning
 *  This does not modify the sentinel node, just the one at the start of the
 *  list.
 *
 * @param head  The sentinel head of the list.
 * @param node  The abstract node to add.
 */
void node_add_head(struct oo_node *head, struct oo_node *node);

/************************************************************************************
 * Helper functions for specific type conversions.
 *
 * Technically, these are better implemented as macros but they're here for
 * clarity and practice.
 ***********************************************************************************/
/**
 * Get a string db item from its node.
 *
 * @param node  The abstract node to convert.
 *
 * @return the db_string struct corresponding to the abstract node.
 */
struct db_string *node_to_dbstr(struct oo_node *node);

/**
 * Get a ulong db item from its node.
 *
 * @param node  The abstract node to convert.
 *
 * @return the db_ulong struct corresponding to the abstract node.
 */
struct db_ulong *node_to_dbulong(struct oo_node *node);

/**
 * Add a record to a database, regardless of its type.
 *
 * @param db    The database to add the record to.
 * @param node  The node to add to the database.
 */
void db_add_record(struct db *db, struct oo_node *node);

/**
 * An example of where macros are useful, but a bit annoying.
 *
 * Example:
 * \code{.c}
 * struct db_ulong ull;
 * struct db_string up;
 *
 * node_mk_node(&ull.node);
 * node_mk_node(&up.node);
 *
 * db_add_record_type(db_of_ull, &ull);
 * db_add_record_type(db_of_strings, &up);
 * \endcode
 *
 * This will save us some typing and defining functions that are not really
 * super useful and can avoid the penalty of calling a function. However, a
 * user can call the macro incorrectly and thus cause a segmentation fault on a
 * good day. Debugging then can be a smudge harder because of these macros.
 */
#define db_add_record_type(db, record)                                         \
  do {                                                                         \
    db_add_record(db, (struct oo_node *)record);                               \
  } while(0);

/**
 * Create and intialize a new db struct.
 *
 * @param name    The name of the database.
 * @param rtype   The type of records in the db.
 */
struct db *mk_db(const char *name, int rtype);

/**
 * Join the elements of a string db separate by spaces.
 *
 * @param db  The string database to join.
 *
 * @return An allocated string of all database entries joined with spaces.
 */
char *join_str_db(struct db *db);

/**
 * Remove from a ulong database all entries that match a certain value.
 *
 * Reach out to Mohammad if you noticed anything about this function's name \m/
 *
 * @param db    The database to search and destroy.
 * @param value The value to look for.
 *
 * @return the number of matched and destroyed elements.
 */
int searching_seek_and_destroy(struct db *db, unsigned long value);

#endif /* db.h */
