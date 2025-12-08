/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/
#ifndef __VECTOR_H
#define __VECTOR_H

#include <unistd.h>

#define VEC_DEF_CAP 4 /**< The default capacity of a new vector */

/**
 * The main vector structure.
 */
struct vector {
  ssize_t len;        /**< The current length of the vector. */
  ssize_t cap;        /**< The maximum capacity of the vector. */
  unsigned int *data; /**< The data container as an array. */
};

/**
 * Create a new empty vector.
 *
 * The new vector should be empty with default capacity at VEC_DEF_CAP. Your
 * code should make sure that the array in initially zero'd out.
 *
 * Example:
 * \code{.c}
 * struct vector *myvec = vec_new();
 * \endcode
 *
 * @return An allocated struct vector with propoer intial values, NULL on
 *  error.
 */
struct vector *vec_new();

/**
 * Free an already created vector.
 *
 * The memory area for that vector must be completely cleaned up.
 *
 * @param  vec  The vector to free up.
 *
 * @return nothing.
 */
void vec_free(struct vector *vec);

/**
 * Push an element at to the end of the vector.
 *
 * The length or size of the vector should grow as a reason of this operation.
 * Note that in some situations, the vector will need to grow in memory.
 *
 * @param  vec   The vector on which to perform the operation.
 * @param  elem  The element to push at the end of the vector.
 *
 * @return nothing.
 */
void vec_push_back(struct vector *vec, unsigned int elem);

/**
 * Remove the element at the end of the vector.
 *
 * The length or size of the vector should shrink after this operation. Note
 * that you do not need to free up memory if the vector shrinks.
 *
 * @param vec The vector on which to perform the operation.
 *
 * @return the element at the end of the vector.
 */
unsigned int vec_pop_back(struct vector *vec);

/**
 * Get an element from the vector at a certain index.
 *
 * This simply accesses the vector at a specific location and returns the
 * element at that location. No changes to the vector should be observed.
 *
 * If the index integer is out of bounds, your function should set the
 * appropriate error number (see `man errno`).
 *
 * @param vec The vector on which to perform the operation.
 * @param i   The index at which to access the element.
 *
 * @warn The valler of this function should always set `errno` to 0 before
 *  calling this function to make sure that no errors have happened.
 *
 * Example:
 * \code{.c}
 * errno = 0;
 * unsigned int x = vec_elem_at(vec, i);
 * if(errno) {
 *   perror("vec_elem_at");
 *   // x's value is invalid, do not use it.
 * }
 * \endcode
 *
 * @return the element at index i if valid, grabage and set errno on failure.
 */
unsigned int vec_elem_at(struct vector *vec, unsigned int i);

/**
 * Set an element from the vector at a certain index.
 *
 * This function will updated the content of the vector at index `i` to become
 * the element `elem`.
 *
 * If `i` is out of bounds, then the function will return -1 and set the
 * `errno` variable.
 *
 * @param vec   The vector on which to perform this operation.
 * @param i     The index at which to set the element.
 * @param elem  The ement value to update.
 *
 * @return 0 on success, -1 on failure.
 *  On failure, your code should set the appropriate value for `errno`.
 */
int vec_set_at(struct vector *vec, unsigned int i, unsigned int elem);

#endif /* vector.h */
