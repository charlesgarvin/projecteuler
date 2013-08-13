#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define _GNU_SOURCE
#include <unistd.h>
#include <getopt.h>

#define DEFAULT_MAX   1000
#define RANGE_MAX     100000

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
    "    max is an integer representing the maximum number (i.e. sum of all",
    "    multiples below <max>).  The value must be between 1 and 100000.  The",
    "    default is 1000",
    NULL
  };

  printf("%s", program);
  for (i = 0; usage[i]; i++) {
    printf("%s\n", usage[i]);
  }
}

static int brute_force(int max)
{
  int i;
  int sum = 0;

  for (i = 1; i < max; i++) {
    if (i % 3 == 0 || i % 5 == 0) {
      sum += i;
    }
  }

  return sum;
}

static multsum_fn method_to_func(const char *name)
{
  if (!strcmp(name, "brute_force")) {
    return brute_force;
  }
  else {
    return NULL;
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

  long val;
  char *endp;

  opts->func = brute_force;
  opts->max = DEFAULT_MAX;

  while ((opt = getopt_long(argc, argv, "", long_opts, &opt_ind)) >= 0) {
    switch (opt) {
      case 1:
        opts->func = method_to_func(optarg);
        if (opts->func == NULL) {
          printf("Invalid method name\n");
          rv = -1;
        }
        break;

      case 2:
        errno = 0;
        val = strtol(optarg, &endp, 10);
        if (errno || *endp || val < 1 || val > RANGE_MAX) {
          printf("Invalid maximum number\n");
          rv = -1;
        }
        else {
          opts->max = val;
        }
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
  int sum;
  struct my_opts opts;

  if (parse_opts(argc, argv, &opts) < 0) {
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }

  sum = opts.func(opts.max);

  printf("sum = %d\n", sum);

  return EXIT_SUCCESS;
}
