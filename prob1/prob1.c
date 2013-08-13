#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <unistd.h>
#include <getopt.h>

#define DEFAULT_MAX   1000

typedef int (*multsum_fn)(int);

struct my_opts {
  multsum_fn func;
  int max;
};

static void print_usage(const char *program)
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

static int parse_opts(int argc, char **argv, struct my_opts *opts)
{
  int rv = 0;
  int opt;
  int opt_ind;
  static struct option long_opts[] = {
    {"method", required_argument, 0, 1},
    {"max",    required_argument, 0, 2},
    {0,        0,                 0, 0}
  };

  opts->func = NULL;
  opts->max = DEFAULT_MAX;

  while ((opt = getopt_long(argc, argv, "", long_opts, &opt_ind)) >= 0) {
    switch (opt) {
      case 1:
        printf("method = %s\n", optarg);
        break;

      case 2:
        printf("max = %s\n", optarg);
        break;

      default:
        rv = -1;
        break;
    }
  }

  return rv;
}

int main(int argc, char **argv)
{
  int i;
  int sum = 0;
  struct my_opts opts;

  if (parse_opts(argc, argv, &opts) < 0) {
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }

  for (i = 1; i < opts.max; i++) {
    if (i % 3 == 0 || i % 5 == 0) {
      sum += i;
    }
  }

  printf("sum = %d\n", sum);

  return EXIT_SUCCESS;
}
