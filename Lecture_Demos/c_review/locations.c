#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
  int x  = 0x01020304;
  char c = 'b';

  int *p   = &x;
  char *cp = &c;

  printf("We can dereference p(%p) and we get %d\n", p, *p);
  printf("We can dereference cpp(%p) and we get %c\n", cp, *cp);

  // ignore this, it's only to silence the compiler
  (void)p;
  (void)cp;

  return EXIT_SUCCESS;
}
