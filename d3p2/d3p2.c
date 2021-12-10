#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int * vals;
    int capacity;
    int count;
} vector_t;

typedef struct {
    int ones;
    int zeroes;
} bit_count_t;

typedef bool (*filter_func_t)(int, int);

int bit_filter(filter_func_t filter_fn, int * values, int size, int bit_pos);

bit_count_t get_bit_distribution(int * vals, int size, int bit_pos);

const int initial_capacity = 16;

bool bit_is_set(int val, int bit_pos);
bool bit_is_not_set(int val, int bit_pos);

int main(int argc, char const *argv[])
{
    vector_t v;
    v.capacity = initial_capacity;
    v.count = 0;
    v.vals = calloc(initial_capacity, sizeof(int));

    char * line = NULL;

    size_t len = 16;
    ssize_t read;
    int width;
    
    while(read = getline(&line, &len, stdin) != -1) {
        if(v.count >= v.capacity) {
            v.vals = reallocarray(v.vals, v.capacity * 2, sizeof(int));
            v.capacity *= 2;
        }

        width = strlen(line);

        v.vals[v.count++] = strtol(line, NULL,2);
    }

    free(line);

    int *ogr, *csr;

    int j = width - 1;
    int size = v.count;

    int oxygen_generation_rating, carbon_scrubbing_rating;

    ogr = calloc(v.count, sizeof(int));
    memcpy(ogr, v.vals, v.count * sizeof(int));

    csr = calloc(v.count, sizeof(int));
    memcpy(csr, v.vals, v.count * sizeof(int));

    free(v.vals);

    while(j >= 0 && size > 1) {
        bit_count_t bits = get_bit_distribution(ogr, size, j);
        size = bit_filter(bits.ones >= bits.zeroes ? bit_is_set : bit_is_not_set, ogr, size, j);
        ogr = reallocarray(ogr, size, sizeof(int));
        j--;
    }
  
    printf("ogr: %d\n", *ogr);
    oxygen_generation_rating = *ogr;

    free(ogr);

    j = width - 1;
    size = v.count;

    while(j >= 0 && size > 1) {
        bit_count_t bits = get_bit_distribution(csr, size, j);
        size = bit_filter(bits.ones >= bits.zeroes ? bit_is_not_set : bit_is_set, csr, size, j);
        csr = reallocarray(csr, size, sizeof(int));
        j--;
    }

    printf("csr: %d\n", *csr);
    carbon_scrubbing_rating = *csr;

    free(csr);

    printf("answer = %d\n", oxygen_generation_rating * carbon_scrubbing_rating);

    return 0;
}

int bit_filter(filter_func_t filter_fn, int * values, int size, int bit_pos)
{
    int ctr = 0;
    for (int i = 0; i < size; i++)
    {
        if (filter_fn(values[i], bit_pos))
        {
            values[ctr] = values[i];
            ctr += 1;
        }
    }

    return ctr;
}

bit_count_t get_bit_distribution(int * vals, int size, int bit_pos) {
    
    bit_count_t result = {
        .ones = 0,
        .zeroes = 0
    };
    
    for(int i = 0; i < size; i++) {
        if (bit_is_set(vals[i], bit_pos)) {
            result.ones++;
        } else {
            result.zeroes++;
        }
    }

    return result;
}

bool bit_is_set(int val, int bit_pos) {
    return (val & (1 << bit_pos)) != 0;
}

// lol
bool bit_is_not_set(int val, int bit_pos) {
    return !bit_is_set(val, bit_pos);
}
