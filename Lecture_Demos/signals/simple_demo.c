#include <stdio.h>

int main(int argc, char **argv) {
  int *p = 0x00;

  printf("Dereferencing p would give us: %d\n", *p);
  printf("What do you think will happen here?\n");

  return 0;
}
