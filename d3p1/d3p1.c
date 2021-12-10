#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

const int initial_capacity = 16;

typedef struct {
    int * vals;
    int capacity;
    int count;
} vector_t;

typedef struct {
    int ones;
    int zeroes;
} bit_count_t;

int main(int argc, char const *argv[])
{
    vector_t v;
    v.capacity = initial_capacity;
    v.count = 0;
    v.vals = (int *)calloc(initial_capacity, sizeof(int));

    char * line = NULL;

    size_t len = 16;
    ssize_t read;
    int width;

    while(read = getline(&line, &len, stdin) != -1) {
        if(v.count >= v.capacity) {
            v.vals = (int*)reallocarray(v.vals, v.capacity * 2, sizeof(int));
            v.capacity *= 2;
        }

        width = strlen(line);

        v.vals[v.count++] = strtol(line, NULL,2);
    }

    free(line);

    bit_count_t * bits = (bit_count_t *)calloc(width, sizeof(bit_count_t));

    // Iterate all numbers
    for(int i = 0; i < v.count; i++) {
        // Get the bit value at each bit position
        for(int j = 0; j < width; j++) {
            if((v.vals[i] & (1 << j)) != 0) {
                bits[j].ones++;
            } else {
                bits[j].zeroes++;
            }
        }
    }

    uint32_t gamma = 0;

    // Iterate each of the bit_count_t objects
    for(int j = 0; j < width; j++) {
        // Determine whether there are more zeroes or ones set
        if (bits[j].ones > bits[j].zeroes) {
            gamma |= (1 << j);
        }
    }

    printf("gamma: %d\n", gamma);

    int epsilon = ~gamma & ((1 << width) - 1);

    printf("epsilon: %d\n", epsilon);

    printf("answer: %d\n", gamma * epsilon);

    return 0;
}
