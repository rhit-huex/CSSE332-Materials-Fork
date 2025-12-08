/**
 * Copyright (c) 2025 Rose-Hulman Institute of Technology. All Rights Reserved.
 *
 * Exploration of a simple array in memory.
 *
 * @author <Your name>
 * @date   <Date last modified>
 */
#include <stdio.h>

static void
dump_memory_bytes(void *start, ssize_t len)
{
  char *p;
  // DO NOT USE THIS FUNCTION IF LEN IS NOT A MULTIPLE OF 4
  for(p = (char *)start; p < (char *)start + len; p += 4) {
    printf("%p:  0x%02x  0x%02x  0x%02x  0x%02x\n", p, *p, *(p + 1), *(p + 2),
           *(p + 3));
  }
}

static void
dump_memory_chars(void *start, ssize_t len)
{
  char *p;
  // DO NOT USE THIS FUNCTION IF LEN IS NOT A MULTIPLE OF 4
  for(p = (char *)start; p < (char *)start + len; p += 4) {
    printf("%p:  %c  %c  %c  %c\n", p, *p, *(p + 1), *(p + 2), *(p + 3));
  }
}

int
main(int argc, char **argv)
{
  char *msg;
  int array_of_integers[] = {1819043144, 1867980911, 560229490, 1750335498};
  printf("1) The elements in the array of integers are [%d, %d, %d, %d]\n\n",
         array_of_integers[0], array_of_integers[1], array_of_integers[2],
         array_of_integers[3]);

  msg = (char *)array_of_integers;
  printf("2) The address array_of_integers is at %p while msg is at %p\n\n",
         array_of_integers, msg);

  printf("3) The content of the message are: %s\n", msg);

  // TODO: Print out the content of memory as simple bytes
  printf("4) Dumping the content of memory starting from array_of_integers:\n");
  // ----- # REMOVE FOR RELEASE # ----- //
  dump_memory_bytes(array_of_integers, 4 * sizeof(int));
  // ----- # END REMOVE FOR RELEASE # ----- //

  // TODO: Print out the content of memory as ASCII bytes
  printf(
      "\n5) Dumping the content of memory starting from array_of_integers as "
      "ASCII:\n");
  // ----- # REMOVE FOR RELEASE # ----- //
  dump_memory_chars(array_of_integers, 4 * sizeof(int));
  // ----- # END REMOVE FOR RELEASE # ----- //

  return 0;
}
