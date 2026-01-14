#include <stdio.h>

int
main(int argc, char *argv[])
{
  FILE *fp;
  char buff;

  fp = fopen("test.txt", "r");

  for(int i = 0; i < 8192; i++)
    fread(&buff, 1, 1, fp);

  return 0;
}
