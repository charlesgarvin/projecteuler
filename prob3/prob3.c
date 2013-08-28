#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

#include "mymalloc.h"
#include "sieve_eratosthenes.h"

bool is_prime(uint64_t num)
{
    uint64_t i;
    uint64_t stop;

    if (num < 2) {
        return false;
    }
    else if (num == 2) {
        return true;
    }
    else if (num % 2 == 0) {
        return false;
    }

    stop = ceil(sqrt(num));
    for (i = 3; i <= stop; i += 2) {
        if (num % i == 0) {
            return 0;
        }
    }

    return true;
}

uint64_t remove_prime_factors(uint64_t num, uint64_t prime_factor)
{
    while (num % prime_factor == 0) {
        num /= prime_factor;
    }

    return num;
}

uint64_t largest_prime_factor(uint64_t num)
{
    uint64_t i;
    uint64_t max;
    uint64_t newnum;

    max = 0;
    for (i = 2; i <= num; i++) {
        if (is_prime(i)) {
            newnum = remove_prime_factors(num, i);
            if (num != newnum) {
                max = i;
                num = newnum;
                printf("Removed %llu, new num = %llu\n",
                       (unsigned long long) i, (unsigned long long) num);
            }
        }
    }

    return max;
}

int main(int argc, char **argv)
{
    int i;
    char *endp;
    uint64_t x;
    uint64_t lpf;

    for (i = 1; i < argc; i++) {
        errno = 0;
        x = strtoull(argv[i], &endp, 10);
        if (errno || *endp) {
            fprintf(stderr, "arg %d is not a valid unsigned integer: [%s]\n", i, argv[i]);
            continue;
        }
        lpf = largest_prime_factor(x);
        printf("The largest prime factor of %llu is %llu\n",
               (unsigned long long) x, (unsigned long long) lpf);
    }

    return 0;
}
