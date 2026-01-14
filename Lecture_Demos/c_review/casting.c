#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(void)
{
  int i    = 0;
  int *p   = malloc(5 * sizeof(int));
  p[4]     = 719;
  char *cp = (char *)p;

  printf("Are p and cp any different? p = %p, and cp = %p\n", p, cp);

  // In cp, we now have an array of 5 * sizeof(int) = 20 characters, we can
  // even use it as a string.
  strncpy(cp, "Hello World!", strlen("Hello World!") + 1);
  printf("cp as a string is: %s\n", cp);

  // We have also still access p as an array of integers.
  for(i = 0; i < 5; i++) {
    printf("Array[%d] = %10d (0x%08x)\n", i, p[i], p[i]);
  }

  free(p);
  // WATCH OUT HERE, DO ONLY ONE FREE!
  return EXIT_SUCCESS;
}
