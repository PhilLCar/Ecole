#include <stdio.h>

int main() {
  int a, m;
  for (a = 36, m = 0; a >= 20; m += 10) {
    printf("%3d%3d\n", a, m);
    if (a % 2 == 0)
      a += 9;
    else
      a /= 10;
  } 
  printf("%3d%3d\n", a, m);
  printf("\nAu revoir!\nFin");
  return 0;
}
