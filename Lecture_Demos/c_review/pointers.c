#include <stdio.h>

int
main(int argc, char **argv)
{
  int *p = (int *)0xdeadbeef;

  // silence compilation warnings, ignore this.
  (void)p;

  return 0;
}
