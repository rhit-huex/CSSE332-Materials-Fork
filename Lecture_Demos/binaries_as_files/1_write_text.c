#include <string.h>
#include <stdio.h>

struct map_pin {
  long long id;
  int coordinate[2];
  char msg[8];
};

void main(){
  struct map_pin pin;
  pin.id = 0xEFL;
  pin.coordinate[0]=0x1;
  pin.coordinate[1]=0x5;
  strcpy(pin.msg, "Hello?"); // 48 65 6C 6C 6F 3F
  
  // print struct to console
  printf("id=%llx coordinate[0]=%d coordinate[1]=%d msg=%s\n", pin.id, pin.coordinate[0], pin.coordinate[1], pin.msg);

  // write struct to file
  FILE* file = fopen("text_output.txt", "w");
  fprintf(file, "id=%llx coordinate[0]=%d coordinate[1]=%d msg=%s\n", pin.id, pin.coordinate[0], pin.coordinate[1], pin.msg);
  fclose(file);

  // It would be quite challenging to write C code to READ the above file back into a struct!
}

