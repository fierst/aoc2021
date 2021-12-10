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

void add_fish(vector_t * f, int8_t value);
void rotate_fish_array();

uint64_t f[9] = {0};

int main(int argc, char const *argv[])
{
    uint64_t fishy_sum = 0;

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

    char * tok = strtok(line, ",");

    while(tok != NULL) {

        uint8_t fish_index = (uint8_t)atoi(tok);
        f[fish_index]++;
        tok = strtok(NULL, ",");
    }

    for(int d = 0; d < num_days; d++) {
        rotate_fish_array();
    }

    for(int i = 0; i < 9; i++) {
        fishy_sum += f[i];
    }

    printf("total fish: %ld\n", fishy_sum);

    free(line);

    return 0;
}

void rotate_fish_array() {
    uint64_t ready_to_spawn = f[0];

    for(int i = 0; i < 8; i++) {
        f[i] = f[i+1];
    }

    f[8] = ready_to_spawn;
    f[6] += ready_to_spawn;    
}