#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv)
{
  int *q  = 0;
  int *p  = malloc(10 * sizeof(int));
  char *s = "hello world!";
  char *t = 0, *r = 0;

  printf("The pointer p contains %p, (p+1) is %p\n", p, (p + 1));

  // example iteration through the array
  q = p;
  // Q: What is the value of (p + 10)?
  while(q < p + 10) {
    // Q: what are we doing here, by how many bytes do we move the address in p?
    *q = 2;
    q  = q + 1;
  }

  // Q: What is this loop doing? Why are we comparing s to zero?
  t = malloc(strlen(s) + 1);
  r = t;
  while(*s != 0) {
    *r = toupper(*s);
    r++;
    s++;
    // one line equivalent of the above
    // *r++ = toupper(*s++);
  }
  // Q: What happens if we print s here? How about s + 1?
  printf("After the loop, the string in t is: %s\n", t);

  free(p);
  free(t);
  return EXIT_SUCCESS;
}
