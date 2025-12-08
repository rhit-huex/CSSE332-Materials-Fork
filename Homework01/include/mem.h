/************************************************************************************
 *
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Should you find any bugs in this file, please contact your instructor as
 * soon as possible.
 *
 ***********************************************************************************/
#ifndef _MEM_H
#define _MEM_H

#include <stddef.h>

/**
 * Create an empty slab of memory that can hold a certain amount of types.
 *
 * The allocated slab of memory should have enough room to hold `nc` characters
 * and `ni` integers.
 *
 * The memory area should always be aligned to multiple of 4 bytes.
 *
 * Example:
 * \code{.c}
 * // make room for 2 characters and 3 integers.
 * void *mem = getmem(2, 3);
 * \endcode
 *
 * @param nc  The number of characters to hold.
 * @param ni  The number of integers to hold.
 *
 * @return An area of memory that can hold at least nc characets and ni
 * integers,
 */
void *getmem(int nc, int ni);

/**
 * Free the memory area pointed to by the user's mem pointer.
 *
 * @param mem   The start of the user's memory area.
 *
 * @return nothing.
 */
void freemem(void *mem);

/**
 * Get the number of characters in the slab.
 *
 * @param mem   Pointer to the start of the memory area from the user.
 *
 * @return The number of characters in the slab of memory.
 */
int getnc(void *mem);

/**
 * Get the number of integers in the slab.
 *
 * @param mem   Pointer to the start of the memory area from the user.
 *
 * @return The number of integers in the slab of memory.
 */
int getni(void *mem);

/**
 * Get a char pointer to the start of the string area.
 *
 * @warning
 *   This function does not check for bounds after passing the pointer back.
 *
 * @param mem   Pointer to the start of the memory area from the user.
 *
 * @return the point to the start of the area of memory that holds the
 * characters.
 */
char *getstr(void *mem);

/**
 * Copy a string over into the memory region that has been created.
 *
 * This function will copy over at most `len` bytes from str into the string
 * area of mem. If len is greater than getnc(mem), then this function will copy
 * over at most `getnc(mem)-1` bytes into `mem` and set a null byte at the
 * end.
 *
 * @param mem   Pointer to the start of the memory area from the user.
 * @param str   The string to copy into the memory area.
 * @param len   The length of bytes to copy over.
 *
 * @return The number of bytes copied into mem (include the null byte).
 */
size_t cpstr(void *mem, const char *str, size_t len);

/**
 * Get an pointer to the start of the interger area in mem.
 *
 * @param mem   The start of the user's memory slab.
 *
 * @return a pointer to the start of the integer area of mem, 0 if it doesn't
 *         exists.
 */
int *getintptr(void *mem);

/**
 * Get the integer at a specific location from the memory area.
 *
 * @param mem   The start of the user's memory slab.
 * @param idx   The index of of the integer to get.
 * @param res   Integer pointer where to store the result.
 *
 * @return 0 on success, -1 on failure.
 */
int getint_at(void *mem, int idx, int *res);

/**
 * Set the integer at a specific location from the memory area.
 *
 * @param mem   The start of the user's memory slab.
 * @param idx   The index of of the integer to set.
 * @param val   The value to store at that index.
 *
 * @return 0 on success, -1 on failure.
 */
int setint_at(void *mem, int idx, int val);

#endif /* mem.h */
