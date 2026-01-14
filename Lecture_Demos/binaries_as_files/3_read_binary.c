#include <string.h>
#include <stdio.h>

struct map_pin {
  long long id;
  int coordinate[2];
  char msg[8];
};

void main(){
  FILE* file = fopen("binary_output.bin", "r");
  struct map_pin read_pin;
  fread(&read_pin, sizeof(struct map_pin), 1, file);
  
  printf("id=%llx coordinate[0]=%d coordinate[1]=%d msg=%s\n", read_pin.id, read_pin.coordinate[0], read_pin.coordinate[1], read_pin.msg);
  
}

