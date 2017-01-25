#include <stdio.h>
#include <stdlib.h>

struct bigint {
  int sign = 0;
  int length = 0;
  int* value;
};

int checkint(bigint a)
{
  if (!a.length) {
    a.value = malloc(1);
    if (!bigint.value)
  }
  
}

void randomfill(bigint a, int size)
{
}

int* add(int* bigint1, int* bigint2) {
  int i = 0;
  int lb1 = *(bigint1);
  int lb2 = *(bigint2);
  long result;
  long carry;
  for (i = 1; i < lb1 && i < lb2; i++) {
    result = *(bigint1 + i) + *(bigint2 + i);
    
  }
}

int* sub;

int* mul;

int* div;

int* factor(int* num)
{
  div(num, 2);
  
}

int isprime(int* num, int prec)
{
  int i, * j, prec;
  int* a, * x;
  int** fctinfo = factor(num);
  int* r = fctinfo[0];
  int* d = fctinfo[1];
  for (i = 0; i < prec; i++) {
  loop:
    a = pickrandom(2, sub(num, 2));
    x = powermod(a, d, num);
    if(equal(x, one) || equal(x, n - 1)) continue;
    for (set(j, 0); smaller(j, sub(r, 1)); add(j, 1)) {
      x = powermod(x, 2, num);
      if (equal(x, one)) return 0;
      if (equal(x, sub(num, 1))) {
	i++;
	if (i < prec) goto loop;
	return 1;
      }
    }
    return 0;
  }
  return 1;
}

int* bigint(int integer)
{
  int* ret = malloc(2);
  *(ret) = 1;
  *(ret + 1) = integer;
  return ret;
}
