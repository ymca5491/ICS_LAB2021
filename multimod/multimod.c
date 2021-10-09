#include <stdint.h>

uint64_t mod(uint64_t a, uint64_t m) {
  uint64_t half_a = a >> 1;
  uint64_t t_m = m;
  while(half_a >= t_m) t_m <<= 1;
  while (a >= m) {
    if (a >= t_m) a -= t_m;
    t_m >>= 1;
  }
  return a;
}

uint64_t plusmod(uint64_t a, uint64_t b, uint64_t m) {
  /* a, b < m */
  uint64_t t_ans = a + b;
    if (t_ans < a || t_ans >= m) {
      t_ans -= m;
    }
    return t_ans;
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
  uint64_t ans = 0;
  a = mod(a, m);
  for (; b; b >> 1) {
    if (b & 1) {
      ans = plusmod(ans, a, m);
    }
    a = plusmod(a, a, m);
  }
  return ans;
}
