#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY    16
#define IS_VERTICAL(line)   (line.x1 == line.x2)
#define IS_HORIZONTAL(line) (line.y1 == line.y2)
#define IS_DIAGONAL_Q1(line)    (line.x1 < line.x2 && line.y1 < line.y2)
#define IS_DIAGONAL_Q2(line)    (line.x1 > line.x2 && line.y1 < line.y2)
#define IS_DIAGONAL_Q3(line)    (line.x1 > line.x2 && line.y1 > line.y2)
#define IS_DIAGONAL_Q4(line)    (line.x1 < line.x2 && line.y1 > line.y2)

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
} line_t;

typedef struct {
    int ** values;
    int width;
    int height;
} grid_t;

typedef struct {
    void * items;
    int count;
    int capacity;
} vector_t;

line_t parse_line_from_stdin(const char * s); 
int read_number(const char ** s);
void draw_grid(grid_t * g);
int get_grid_points(grid_t * g);

int main(int argc, char const *argv[])
{
    vector_t lines = {
        .count = 0,
        .capacity = INITIAL_CAPACITY,
        .items = calloc(INITIAL_CAPACITY, sizeof(line_t))
    };

    char * line = NULL;
    size_t len;
    ssize_t read;

    while(read = getline(&line, &len, stdin) != -1) {
        if (lines.count >= lines.capacity) {
            lines.capacity *= 2;
            lines.items = reallocarray(lines.items, lines.capacity, sizeof(line_t));
        }
        const char * p = line;
        ((line_t *)lines.items)[lines.count++] = parse_line_from_stdin(p);
    }

    int min_x = __INT_MAX__;
    int min_y = __INT_MAX__;
    int max_x = 0;
    int max_y = 0;

    // Find bounds of lines to get size of grid
    for(int i = 0; i < lines.count; i++) {
        // Get minimum x and y values
        if(((line_t *)lines.items)[i].x1 < min_x)   min_x = ((line_t *)lines.items)[i].x1;
        if(((line_t *)lines.items)[i].x2 < min_x)   min_x = ((line_t *)lines.items)[i].x2;
        if(((line_t *)lines.items)[i].y1 < min_y)   min_y = ((line_t *)lines.items)[i].y1;
        if(((line_t *)lines.items)[i].y2 < min_y)   min_x = ((line_t *)lines.items)[i].y2;

        // Get maximum x and y values
        if(((line_t *)lines.items)[i].x1 > max_x)   max_x = ((line_t *)lines.items)[i].x1;
        if(((line_t *)lines.items)[i].x2 > max_x)   max_x = ((line_t *)lines.items)[i].x2;
        if(((line_t *)lines.items)[i].y1 > max_y)   max_y = ((line_t *)lines.items)[i].y1;
        if(((line_t *)lines.items)[i].y2 > max_y)   max_x = ((line_t *)lines.items)[i].y2;
    }

    for(int i = 0; i < lines.count; i++) {
        ((line_t *)lines.items)[i].x1 -= min_x;
        ((line_t *)lines.items)[i].x2 -= min_x;
        ((line_t *)lines.items)[i].y1 -= min_y;
        ((line_t *)lines.items)[i].y2 -= min_y;
    }

    // Allocate space for the grid
    grid_t grid = {
        .width = (max_x - min_x) + 1,
        .height = (max_y - min_y) + 1
    };

    grid.values = calloc(grid.height, sizeof(*grid.values));
    for(int i = 0; i < grid.height; i++) {
        grid.values[i] = calloc(grid.width, sizeof(*grid.values[i]));
    }

    for(int i = 0; i < lines.count; i++) {
        if(IS_VERTICAL(((line_t *)lines.items)[i])) {
            int x = ((line_t *)lines.items)[i].x1;
            if(((line_t *)lines.items)[i].y1 > ((line_t *)lines.items)[i].y2) {
                for(int y = ((line_t *)lines.items)[i].y2; y <= ((line_t *)lines.items)[i].y1; y++) {
                    grid.values[y][x]++;
                }
            } else {
                for(int y = ((line_t *)lines.items)[i].y1; y <= ((line_t *)lines.items)[i].y2; y++) {
                    grid.values[y][x]++;
                }
            }
        } else if (IS_HORIZONTAL(((line_t *)lines.items)[i])) {
            int y = ((line_t *)lines.items)[i].y1;
            if(((line_t *)lines.items)[i].x1 > ((line_t *)lines.items)[i].x2) {
                for(int x = ((line_t *)lines.items)[i].x2; x <= ((line_t *)lines.items)[i].x1; x++) {
                    grid.values[y][x]++;
                }
            } else {
                for(int x = ((line_t *)lines.items)[i].x1; x <= ((line_t *)lines.items)[i].x2; x++) {
                    grid.values[y][x]++;
                }
            }
        } else if (IS_DIAGONAL_Q1(((line_t *)lines.items)[i])) {
            // Diagonal line with both x and y increasing
            int x = ((line_t *)lines.items)[i].x1;
            int y = ((line_t *)lines.items)[i].y1;
            int len = 1 + (((line_t *)lines.items)[i].x2 - x);
            for(int i = 0; i < len; i++) {
                grid.values[y+i][x+i]++;
            }
        } else if (IS_DIAGONAL_Q2(((line_t *)lines.items)[i])) {
            // Diagonal line with x decreasing and y increasing
            int x = ((line_t *)lines.items)[i].x1;
            int y = ((line_t *)lines.items)[i].y1;
            int len = 1 + (x - ((line_t *)lines.items)[i].x2);
            for(int i = 0; i < len; i++) {
                grid.values[y+i][x-i]++;
            }        
        } else if (IS_DIAGONAL_Q3(((line_t *)lines.items)[i])) {
            // Diagonal line with both x and y decreasing
            int x = ((line_t *)lines.items)[i].x1;
            int y = ((line_t *)lines.items)[i].y1;
            int len = 1 + (x - ((line_t *)lines.items)[i].x2);
            for(int i = 0; i < len; i++) {
                grid.values[y-i][x-i]++;
            }        
        } else if (IS_DIAGONAL_Q4(((line_t *)lines.items)[i])) {
            // Diagonal line with x increasing and y decreasing
            int x = ((line_t *)lines.items)[i].x1;
            int y = ((line_t *)lines.items)[i].y1;
            int len = 1 + (((line_t *)lines.items)[i].x2 - x);
            for(int i = 0; i < len; i++) {
                grid.values[y-i][x+i]++;
            }        
        }

        // printf("***%d***\n", i);
        // draw_grid(&grid);
    }

    printf("\n*** ANSWER: %d ***", get_grid_points(&grid));    

    free(line);
    free(lines.items);

    for(int i = 0; i < grid.height; i++) {
        free(grid.values[i]);
    }

    free(grid.values);
    
    return 0;
}

line_t parse_line_from_stdin(const char * s) {
    line_t l;
    l.x1 = read_number(&s);
    l.y1 = read_number(&s);
    l.x2 = read_number(&s);
    l.y2 = read_number(&s);
    return l;
}

int read_number(const char ** s) {
    
    int val = 0;

    char * p = *s;

    // skip all leading spaces, arrow characters, or commas
    while(*p == ' ' || *p == '-' || *p == '>' || *p == ',') {
        ++p;
    }

    while (*p >= '0' && *p <= '9') {               /* Parse until character is valid number */
        val = val * 10 + (*p - 0x30);
        ++p;
    }

    *s = p;
    
    return val;
}

void draw_grid(grid_t * g) {
    for(int r = 0; r < g->height; r++) {
        for(int c = 0; c < g->width; c++) {
            if(g->values[r][c] == 0) {
                printf(". ");
            } else {
                printf("%d ", g->values[r][c]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

int get_grid_points(grid_t * g) {
    int ret = 0;

    for (int r = 0; r < g->height; r++) {
        for(int c = 0; c < g->width; c++) {
            if (g->values[r][c] > 1) {
                ret++;
            }
        }
    }    
    return ret;
}