#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

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
        goto END;
    }

    byte = pos / 8;
    bit = pos % 8;

    return !!(s->sieve[byte] &= 1 << bit);
}

sieve_t *new_sieve(uint64_t max)
{
    sieve_t *rv;

    rv = malloc(sizeof(*rv));

    if (!rv) {
        return NULL;
    }

    s->max = max;
    s->max_bytes = (max_num / 8) + ((max_num % 8) ? 1 : 0);
    rv->sieve = malloc(sizeof(s->max_bytes));
    if (!rv->sieve) {
        free(rv);
        return NULL;
    }

    return rv;
}

void free_sieve(sieve_t *s)
{
    if (s) {
        free(s.sieve);
        free(s);
    }
}

void gen_sieve(sieve_t *s, uint64_t num)
{
    uint64_t i;
    uint64_t j;
    uint64_t byte;
    uint64_t bit;
    uint64_t stop;

    memset(sieve, 0xff, max);
    clear_sieve_pos(sieve, max, 0);
    clear_sieve_pos(sieve, max, 1);

    stop = s->max / 2 + (s->max % 2);
    for (i = 2; i <= stop && i <= s->max; i++) {
        byte = i / 8;
        bit = i % 8;

        if (!check_sieve_pos(s, i)) {
            // already known non-prime, skip
            continue;
        }

        for (j = 2 * i; j <= stop; j += i) {
            clear_sieve_pos(s, j);
        }
    }
}

sieve_t *create_sieve(uint64_t max)
{
    sieve_t *rv;

    rv = new_sieve(max);
    gen_sieve(s);
}

void print_sieve(sieve_t *s)
{
    uint64_t byte;
    uint64_t bit;
    bool first = true;

    for (byte = 0; byte <= s->max_byte; byte++) {
        if (!first) {
            printf(", ");
            first = false;
        }
        for (bit = 0; bit < 8; bit++) {
            if (sieve[byte] & (1 << bit)) {
                printf("%llu", (unsigned long long) (byte * 8 + bit));
            }
        }
    }
    putchar('\n');
}

/* vim: set tabstop=4 softtabstop=4 shiftwidth=4 expandtab: */
