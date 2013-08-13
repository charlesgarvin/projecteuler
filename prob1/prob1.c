#include <stdio.h>

#define MAX   1000

int main(void)
{
  int i;
  int sum = 0;

  for (i = 1; i < MAX; i++) {
    if (i % 3 == 0 || i % 5 == 0) {
      printf("%d\n", i);
      sum += i;
    }
  }

  printf("sum = %d\n", sum);

  return 0;
}
