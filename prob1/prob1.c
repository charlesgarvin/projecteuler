#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define _GNU_SOURCE
#include <unistd.h>
#include <getopt.h>

#define DEFAULT_MAX   1000
#define RANGE_MAX     1000000000ULL

typedef unsigned long long (*multsum_fn)(unsigned long long);

struct my_opts {
  multsum_fn func;
  unsigned long long max;
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

static unsigned long long brute_force(unsigned long long max)
{
  unsigned long long i;
  unsigned long long sum = 0;

  for (i = 1; i < max; i++) {
    if (i % 3 == 0 || i % 5 == 0) {
      sum += i;
    }
  }

  return sum;
}

static unsigned long long generate(unsigned long long max)
{
  unsigned long long sum = 0;
  unsigned long long three = 3;
  unsigned long long five = 5;
  unsigned long long prev_three = 0;
  unsigned long long prev_five = 0;

  while (three < max || five < max) {
    if (three < max && three != five && three != prev_three) {
      // only count multiples of 3 less than max, if it's not also a mult of 5,
      // and we haven't counted it before
      sum += three;
    }
    if (five < max && three != five && five != prev_five) {
      // only count multiples of 5 less than max, if it's not also a mult of 3,
      // and we haven't counted it before
      sum += five;
    }

    // set prev values so we don't count the same number twice
    prev_three = three;
    prev_five = five;

    if (three < five) {
      // multiples of 3 are lagging, increment
      three += 3;
    } else if (five < three) {
      // multiples of 5 are lagging, increment
      five += 5;
    } else {
      // multiple of 3 and 5, increment both
      three += 3;
      five += 5;
    }
  }

  return sum;
}

static unsigned long long mult_sum(unsigned long long mult, unsigned long long max)
{
  unsigned long long div;

  div = max / mult;

  return mult * (div * div + div) / 2;
}

static unsigned long long const_time(unsigned long long max)
{
  unsigned long long three_sum;
  unsigned long long five_sum;
  unsigned long long common_sum;

  max -= 1;  // don't count max itself
  three_sum = mult_sum(3, max);
  five_sum = mult_sum(5, max);
  common_sum = mult_sum(3 * 5, max);

  return three_sum + five_sum - common_sum;
}

static multsum_fn method_to_func(const char *name)
{
  if (!strcmp(name, "brute_force")) {
    return brute_force;
  } else if (!strcmp(name, "generate")) {
    return generate;
  } else if (!strcmp(name, "const_time")) {
    return const_time;
  } else {
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
  unsigned long long sum;
  struct my_opts opts;

  if (parse_opts(argc, argv, &opts) < 0) {
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }

  sum = opts.func(opts.max);

  printf("sum = %llu\n", sum);

  return EXIT_SUCCESS;
}
