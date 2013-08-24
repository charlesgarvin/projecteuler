#ifndef sieve_eratosthenes_c__
#define sieve_eratosthenes_c__

#include <stdint.h>
#include <stdbool.h>

typedef struct sieve_s    sieve_t;

void clear_sieve_pos(sieve_t *s, uint64_t pos);
void set_sieve_pos(sieve_t *s, uint64_t pos);
bool check_sieve_pos(sieve_t *s, uint64_t pos);
sieve_t *new_sieve(uint64_t max);
void free_sieve(sieve_t *s);
void gen_sieve(sieve_t *s);
sieve_t *create_sieve(uint64_t max);
void print_sieve(sieve_t *s);

#endif  // sieve_eratosthenes_c__
