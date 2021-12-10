#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

#define N 10000000

static bool is_prime[N];
static int  primes[N];

int *sieve(int n) {
  int sqrt_n = sqrt(n);
  assert(n + 1 < N);
  for (int i = 0; i <= n; i++) {
    if (i & 1)
      is_prime[i] = true;
    else 
      is_prime[i] = false;
  }
  is_prime[1] = false;
  is_prime[2] = true;

  for (int i = 3; i <= sqrt_n; i += 2) {
    if (is_prime[i]) {
      for (int j = i * i; j <= n; j += i) {
        is_prime[j] = false;
      }
    }
  }

  int *p = primes;
  for (int i = 2; i <= n; i++)
    if (is_prime[i]) {
      *p++ = i;
    }
  *p = 0;
  return primes;
}
