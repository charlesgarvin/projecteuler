#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define _GNU_SOURCE
#include <unistd.h>
#include <getopt.h>

#define DEFAULT_MAX   4000000
#define RANGE_MAX     1000000000ULL

typedef unsigned long long (*fib_even_sum_fn)(unsigned long long);

struct my_opts {
  fib_even_sum_fn func;
  unsigned long long max;
};

static unsigned long long fib_recur(unsigned long long n)
{
  if (n == 0) {
    return 0;
  }
  else if (n == 1) {
    return 1;
  }

  return fib_recur(n - 1) + fib_recur(n - 2);
}

static unsigned long long recursive(unsigned long long max)
{
  unsigned long long i = 0;
  unsigned long long n = 0;
  unsigned long long sum = 0;

  for (i = 0; n < max; i++) {
    n = fib_recur(i);
    if (n % 2 == 0) {
      sum += n;
    }
  }

  return sum;
}

static unsigned long long generative(unsigned long long max)
{
  unsigned long long n = 0;
  unsigned long long prev_1 = 1;
  unsigned long long prev_2 = 0;
  unsigned long long sum = 0;

  while (n < max) {
    n = prev_1 + prev_2;
    if (n % 2 == 0) {
      sum += n;
    }
    prev_2 = prev_1;
    prev_1 = n;
  }

  return sum;
}

static fib_even_sum_fn method_to_func(const char *name)
{
  if (!strcmp(name, "recursive")) {
    return recursive;
  }
  if (!strcmp(name, "generative")) {
    return generative;
  }
  else {
    return NULL;
  }
}

static void print_usage(const char *program)
{
  int i;
  const char *usage[] = {
    " [--method=<method>] [--max=<max>]",
    "",
    "    method is the computation method you want to use.  Valid choices are",
    "    recursive, generative.  The default is recursive.",
    "",
    "    max is an integer representing the maximum number (i.e. sum of all",
    "    even Fibonacci numbers below <max>).  The value must be between 2",
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
    {"method", required_argument, 0, 1},
    {"max",    required_argument, 0, 2},
    {0,        0,                 0, 0}
  };

  long val;
  char *endp;

  opts->func = recursive;
  opts->max = DEFAULT_MAX;

  while ((opt = getopt_long(argc, argv, "", long_opts, &opt_ind)) >= 0) {
    switch (opt) {
      case 1:
        opts->func = method_to_func(optarg);
        if (opts->func == NULL) {
          printf("Invalid method\n");
          rv = -1;
        }
        break;

      case 2:
        errno = 0;
        val = strtoull(optarg, &endp, 10);
        if (errno || *endp || val < 2 || val > RANGE_MAX) {
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

  sum = opts.func(opts.max);

  printf("sum = %llu\n", sum);

  return EXIT_SUCCESS;
}
