#include <stdio.h>
#include <stdlib.h>

#define MAX   1000

void print_usage(const char *program)
{
  int i;
  const char *usage[] = {
    " [--method=<method>] [--max=<max>]",
    "",
    "    <method> is the computation method you wish to use,",
    "    brute_force, sieve or generate.  The default is brute_force.",
    "",
    "    max is the maximum number (i.e. sum of all multiples below <max>.",
    "    The default is 1000",
    NULL
  };

  printf("%s", program);
  for (i = 0; usage[i]; i++) {
    printf("%s\n", usage[i]);
  }
}

int parse_opts(int argc, char **argv)
{
  return -1;
}

int main(int argc, char **argv)
{
  int i;
  int sum = 0;

  if (parse_opts(argc, argv) < 0) {
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }

  for (i = 1; i < MAX; i++) {
    if (i % 3 == 0 || i % 5 == 0) {
      sum += i;
    }
  }

  printf("sum = %d\n", sum);

  return EXIT_SUCCESS;
}
