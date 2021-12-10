#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INITIAL_CAPACITY    16
#define NEW_FISH            8
#define FRESH_FISH          6

typedef struct {
    int8_t * items;
    uint32_t capacity;
    uint32_t count;
} vector_t;

void iterate_fish(vector_t * f);
void add_fish(vector_t * f, int8_t value);
// uint64_t sum_fish(vector_t * f);

int main(int argc, char const *argv[])
{
    if(argc < 2) {
        printf("need number of days!\n");
        return -1;
    }

    int num_days = atoi(argv[1]);

    char * line = NULL;
    size_t len;
    ssize_t read;
    
    read = getline(&line, &len, stdin);
    if(read == -1) {
        printf("idk what happened, no file?\n");
        return -2;
    }

    vector_t fish = {
        .capacity = INITIAL_CAPACITY,
        .count = 0,
        .items = calloc(INITIAL_CAPACITY, sizeof(int8_t))
    };

    char * tok = strtok(line, ",");

    while(tok != NULL) {

        add_fish(&fish, (int8_t)atoi(tok));
        
        tok = strtok(NULL, ",");
    }

    int d = 0;

    while(d < num_days) {
        printf("day %d\n", d);
        iterate_fish(&fish);
        d++;
    }

    printf("total fish: %d\n", fish.count);

    free(line);
    free(fish.items);

    return 0;
}

void add_fish(vector_t * fish, int8_t value) {
    fish->items[fish->count++] = value;
    if(fish->count == fish->capacity) {
        fish->capacity *= 2;
        fish->items = reallocarray(fish->items, fish->capacity, sizeof(int8_t));
    }
}

// uint64_t sum_fish(vector_t * f) {
//     uint64_t sum = 0;

//     for(int i = 0; i < f->count; i++) {
//         sum += f->items[i];
//     }

//     return sum;
// }

void iterate_fish(vector_t * f) {

    for(int i = 0; i < f->count; i++) {
        f->items[i]--;
    }

    for(int i = 0; i < f->count; i++) {
        if(f->items[i] == -1) {
            f->items[i] = 6;
            add_fish(f, NEW_FISH);
        }
    }
}
