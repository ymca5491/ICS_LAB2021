#include "asm.h"
#include <assert.h>
#include <stdio.h>

int main() {
  //asm_jmp_buf buf;
  int a = 0;
  int b = 0xffff;
  //int r = asm_setjmp(buf);
  int r = 0;
  if (r == 0) {
    assert(asm_add(1234, 5678) == 6912);
    assert(asm_popcnt(0x0123456789abcdefULL) == 32);
    assert(*(int*)asm_memcpy(&a, &b, 2) == 0xffff);
    //asm_longjmp(buf, 123);
  } else {
    assert(r == 123);
    printf("PASSED.\n");
  }
}
