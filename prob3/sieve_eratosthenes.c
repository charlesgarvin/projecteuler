#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "mymalloc.h" 
#include "sieve_eratosthenes.h"

struct sieve_s {
    unsigned char *sieve;
    uint64_t max;
    uint64_t max_bytes;
};

void clear_sieve_pos(sieve_t *s, uint64_t pos)
{
    uint64_t byte;
    uint64_t bit;

    if (pos > s->max) {
        return;
    }

    byte = pos / 8;
    bit = pos % 8;

    s->sieve[byte] &= ~(1 << bit);
}

void set_sieve_pos(sieve_t *s, uint64_t pos)
{
    uint64_t byte;
    uint64_t bit;

    if (pos > s->max) {
        return;
    }

    byte = pos / 8;
    bit = pos % 8;

    s->sieve[byte] |= 1 << bit;
}

bool check_sieve_pos(sieve_t *s, uint64_t pos)
{
    uint64_t byte;
    uint64_t bit;

    if (pos > s->max) {
        return false;
    }

    byte = pos / 8;
    bit = pos % 8;

    return !!(s->sieve[byte] & (1 << bit));
}

sieve_t *new_sieve(uint64_t max)
{
    sieve_t *rv;

    rv = mymalloc(sizeof(*rv));

    if (!rv) {
        return NULL;
    }

    rv->max = max;
    rv->max_bytes = (max / 8) + ((max % 8) ? 1 : 0);
    rv->sieve = mymalloc(rv->max_bytes);
    if (!rv->sieve) {
        myfree(rv);
        return NULL;
    }

    return rv;
}

void free_sieve(sieve_t *s)
{
    if (s) {
        myfree(s->sieve);
        myfree(s);
    }
}

void gen_sieve(sieve_t *s)
{
    uint64_t i;
    uint64_t j;
    uint64_t stop;

    memset(s->sieve, 0xff, s->max_bytes);
    clear_sieve_pos(s, 0);
    clear_sieve_pos(s, 1);

    stop = s->max / 2 + (s->max % 2);
    for (i = 2; i <= stop && i <= s->max; i++) {
        putchar('\n');
        if (!check_sieve_pos(s, i)) {
            // already known non-prime, skip
            continue;
        }

        for (j = 2 * i; j <= s->max; j += i) {
            clear_sieve_pos(s, j);
        }
    }
}

sieve_t *create_sieve(uint64_t max)
{
    sieve_t *rv;

    rv = new_sieve(max);
    gen_sieve(rv);

    return rv;
}

void print_sieve(sieve_t *s)
{
    uint64_t i;
    bool first = true;

    for (i = 0; i < s->max; i++) {
        if (check_sieve_pos(s, i)) {
            if (!first) {
                printf(", ");
            }
            printf("%llu", (unsigned long long) i);
            first = false;
        }
    }
    putchar('\n');
}

/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 expandtab: */
