#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define INITIAL_CAPACITY    16

typedef struct {
    char* items;
    int count;
    int capacity;
} vector_t;

static void vector_push_back(vector_t * v, char item);

int main(int argc, char const *argv[])
{
    uint8_t ** map;
    int cols = 0;
    int rows = 0;

    vector_t chars = {
        .capacity = INITIAL_CAPACITY,
        .count = 0,
        .items = malloc(INITIAL_CAPACITY * sizeof(char))
    };
    
    int map_initialized = 0;

    char * line = NULL;
    size_t len;
    ssize_t read;

    while(read = getline(&line, &len, stdin) != EOF) {
        rows++;
        char * p = line;
        while(*p != '\0' && *p != EOF) {
            if(*p == '\n') {
                if(cols == 0) {
                    cols = p - line;
                }
                p++; // skip the newlines...
            }

            if(*p != '\0') {
                vector_push_back(&chars, *p++);
            }            
        }
    }

    // allocate space for the map but leave space around the edge
    // because we're going to surround it with 9 to make our lives easier
    map = calloc(rows + 2, sizeof(*map));
    for(int i = 0; i < rows + 2; i++) {
        map[i] = calloc(cols + 2, sizeof(*map[0]));
    }

    for(int r = 0; r < rows + 2; r++) {
        for(int c = 0; c < cols + 2; c++) {
            if (r == 0 || c == 0 || r == rows + 1 || c == cols + 1) {
                map[r][c] = 9;
            } else {
                map[r][c] = (uint8_t)(chars.items[(cols * (r-1)) + (c-1)] - '0');
            }
            printf("%d", map[r][c]);
        }
        printf("\n");
    }

    int danger_sum = 0;

    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            int v = map[r+1][c+1];
            if( v < map[r][c+1] &&
                v < map[r+1][c] &&
                v < map[r+2][c+1] &&
                v < map[r+1][c+2]) {
                    danger_sum += (1 + map[r+1][c+1]);
                }
        }
    }
    

    free(line);
    for(int i = 0; i < rows + 2; i++) {
        free(map[i]);
    }
    free(map);
    free(chars.items);

    printf("%d\n", danger_sum);

    return 0;
}

static void vector_push_back(vector_t * v, char item) {
    v->items[v->count++] = item;
    if(v->count == v->capacity) {
        v->capacity = v->capacity * 2;
        v->items = reallocarray(v->items, v->capacity, sizeof(v->items[0]));
    }
}