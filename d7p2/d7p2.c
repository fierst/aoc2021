#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16

typedef struct vector_t {
    int * items;
    int count;
    int capacity;
    void (* push_back)(struct vector_t *, int);
} vector_t;

void vector_push_back(vector_t * self, int new_value);
int qsort_compare(const void * arg1, const void * arg2);
int nth_triangle(int num);

int main(int argc, char const *argv[])
{
    vector_t p = {
        .count = 0,
        .capacity = INITIAL_CAPACITY,
        .items = calloc(INITIAL_CAPACITY, sizeof(int)),
        .push_back = vector_push_back
    };

    char * line = NULL;
    size_t len;
    ssize_t read;

    read = getline(&line, &len, stdin);
    if(read == -1) {
        printf("something's wrong\n");
        return -1;
    }

    char * tok = strtok(line, ",");
    
    while(tok != NULL) {

        p.push_back(&p, atoi(tok));

        tok = strtok(NULL, ",");
    }

    qsort(p.items, p.count, sizeof(p.items[0]), qsort_compare);

    uint32_t min_fuel = UINT32_MAX;
    int min_pos = 0;

    for(int i = p.items[0]; i < p.items[p.count - 1]; i++) {
        int fuel = 0;
        for(int c = 0; c < p.count; c++) {
            fuel += nth_triangle(abs(i - p.items[c]));
        }
        //printf("position %d - fuel = %d\n", i, fuel);
        if (fuel < min_fuel) {
            min_fuel = fuel;
            min_pos = i;
        }
    }

    printf("min_fuel @ %d = %d\n", min_pos, min_fuel);

    free(line);
    free(p.items);

    return 0;
}

void vector_push_back(vector_t * self, int new_value) {

    self->items[self->count++] = new_value;

    if(self->count == self->capacity) {
        self->capacity *= 2;
        self->items = reallocarray(self->items, self->capacity, sizeof(int));
    }
}

int qsort_compare(const void * arg1, const void * arg2) {
    if (*(int *)arg1 < *(int *)arg2) {
        return -1;
    } else if (*(int *)arg1 == *(int *)arg2) {
        return 0;
    } else {
        return 1;
    }    
}

int nth_triangle(int num) {
    return ((num * num) + num)/2;
}