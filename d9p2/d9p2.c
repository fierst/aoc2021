#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define INITIAL_CAPACITY    16

typedef struct {
    int row;
    int col;    
} basin_t;

typedef struct {
    char* items;
    int count;
    int capacity;
} char_vector_t;

typedef struct {
    basin_t* items;
    int count;
    int capacity;
} basin_vector_t;

typedef struct {
    int* items;
    int count;
    int capacity;
} vector_t;

static void char_vector_push_back(char_vector_t * v, char item);
static void basin_vector_push_back(basin_vector_t * v, basin_t item);
static void int_vector_push_back(vector_t * v, int item);
static void print_map(uint8_t ** map, int rows, int cols);
static int get_basin_size(uint8_t ** map, int row, int col, int rows, int cols, uint8_t val);
static void init_visited_array(int rows, int cols);
static void clean_visited_array(int rows, int cols);

int comp(const void * a, const void * b)
{
    return *(int *)b - *(int *)a;
}

uint8_t ** visited;

int main(int argc, char const *argv[])
{
    uint8_t ** map;
    int cols = 0;
    int rows = 0;

    char_vector_t chars = {
        .capacity = INITIAL_CAPACITY,
        .count = 0,
        .items = calloc(INITIAL_CAPACITY, sizeof(char))
    };
    
    basin_vector_t basins = {
        .capacity = INITIAL_CAPACITY,
        .count = 0,
        .items = calloc(INITIAL_CAPACITY, sizeof(basin_t))
    };

    vector_t basin_sizes = {
        .capacity = INITIAL_CAPACITY,
        .count = 0,
        .items = calloc(INITIAL_CAPACITY, sizeof(int))
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
                char_vector_push_back(&chars, *p++);
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
        }
    }

    free(line);
    free(chars.items);
    
    // Get the basins
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            int v = map[r+1][c+1];
            if( v < map[r][c+1] &&
                v < map[r+1][c] &&
                v < map[r+2][c+1] &&
                v < map[r+1][c+2]) {
                    basin_t b = {
                        .row = r+1,
                        .col = c+1
                    };
                    basin_vector_push_back(&basins, b);
                }
        }
    }

    // Get the size of the basins
    for(int b = 0; b < basins.count; b++) {
        init_visited_array(rows, cols);
        printf("basin: %d,%d\n", basins.items[b].row, basins.items[b].col);
        int basin_size = get_basin_size(map, basins.items[b].row, basins.items[b].col, rows, cols, map[basins.items[b].row+1][basins.items[b].col+1]);
        clean_visited_array(rows, cols);
        printf("basin size: %d\n", basin_size);
        int_vector_push_back(&basin_sizes, basin_size);
    }

    for(int i = 0; i < rows + 2; i++) {
        free(map[i]);
    }
    free(map);

    free(basins.items);

    qsort(basin_sizes.items, basin_sizes.count, sizeof(int), comp);

    printf("answer: %d\n", basin_sizes.items[0] * basin_sizes.items[1] * basin_sizes.items[2]);

    free(basin_sizes.items);

    return 0;
}

static void char_vector_push_back(char_vector_t * v, char item) {
    v->items[v->count++] = item;
    if(v->count == v->capacity) {
        v->capacity = v->capacity * 2;
        v->items = reallocarray(v->items, v->capacity, sizeof(v->items[0]));
    }
}

static void basin_vector_push_back(basin_vector_t * v, basin_t item) {
    v->items[v->count++] = item;
    if(v->count == v->capacity) {
        v->capacity = v->capacity * 2;
        v->items = reallocarray(v->items, v->capacity, sizeof(v->items[0]));
    }
}

static void int_vector_push_back(vector_t * v, int item) {
    v->items[v->count++] = item;
    if(v->count == v->capacity) {
        v->capacity = v->capacity * 2;
        v->items = reallocarray(v->items, v->capacity, sizeof(v->items[0]));
    }
}

static void print_map(uint8_t ** map, int rows, int cols) {
    for(int r = 0; r < rows + 2; r++) {
        for(int c = 0; c < cols + 2; c++) {
            if(visited[r][c]) {
                printf("\033[0;31m%d\033[0m", map[r][c]);
            } else {
                printf("%d", map[r][c]);
            }
            
        }
        printf("\n");
    }
    printf("\n");
}

static void init_visited_array(int rows, int cols) {
    visited = calloc(rows + 2, sizeof(uint8_t *));
    for(int i = 0; i < rows + 2; i++) {
        visited[i] = calloc(cols + 2, sizeof(uint8_t));
        memset(visited[i], 0, sizeof(uint8_t) * cols + 2);
    }
}

static void clean_visited_array(int rows, int cols) {
    for(int i = 0; i < rows + 2; i++) {
        free(visited[i]);
    }
    free(visited);
}

static int get_basin_size(uint8_t ** map, int row, int col, int rows, int cols, uint8_t val) {
    
    //print_map(map, rows, cols);


    // detect when we're at the end
    if(map[row][col] == 9) {
        visited[row][col] = 1;
    } 
    
    if(visited[row][col]) {
        return 0;
    }

    visited[row][col] = 1;

    int size = 1;

    size += get_basin_size(map, row+1, col, rows, cols, map[row][col]);
    size += get_basin_size(map, row, col+1, rows, cols, map[row][col]);
    size += get_basin_size(map, row-1, col, rows, cols, map[row][col]);
    size += get_basin_size(map, row, col-1, rows, cols, map[row][col]);

    return size;
}