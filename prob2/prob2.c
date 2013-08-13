#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define _GNU_SOURCE
#include <unistd.h>
#include <getopt.h>

#define DEFAULT_MAX   4000000
#define RANGE_MAX     1000000000ULL

struct my_opts {
  unsigned long long max;
};

static void print_usage(const char *program)
{
  int i;
  const char *usage[] = {
    " [--max=<max>]",
    "",
    "    max is an integer representing the maximum number (i.e. sum of all",
    "    even Fibonacci numbers below <max>).  The value must be between 1",
    "    and 1000000000.  The default is 4000000.",
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
    {"max",    required_argument, 0, 1},
    {0,        0,                 0, 0}
  };

  long val;
  char *endp;

  opts->max = DEFAULT_MAX;

  while ((opt = getopt_long(argc, argv, "", long_opts, &opt_ind)) >= 0) {
    switch (opt) {
      case 1:
        errno = 0;
        val = strtoull(optarg, &endp, 10);
        if (errno || *endp || val < 1 || val > RANGE_MAX) {
          printf("Invalid maximum number\n");
          rv = -1;
        } else {
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
  unsigned long long sum = 0;
  struct my_opts opts;

  if (parse_opts(argc, argv, &opts) < 0) {
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }

  printf("sum = %llu\n", sum);

  return EXIT_SUCCESS;
}
