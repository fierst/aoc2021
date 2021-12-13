#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_POINTS  1024
#define MAX_FOLDS   16

enum parse_state {
    DOTS,
    INSTRUCTIONS
};

typedef struct {
    int x;
    int y;
} point_t;

enum direction {
    X,
    Y
};

typedef struct {
    enum direction fold_direction;
    int value;
} fold_t;

point_t points[MAX_POINTS];
int point_count = 0;

static void fold_paper(fold_t* f);

int main(int argc, char const *argv[])
{
    enum parse_state state = DOTS;

    fold_t folds[MAX_FOLDS];
    int fold_count = 0;

    char * line = NULL;
    size_t len;
    ssize_t read;
    
    while(read = getline(&line, &len, stdin) != EOF) {
        char * p = line;

        switch(state) {
            case DOTS:
                if(strlen(p) == 1) {
                    state = INSTRUCTIONS;
                } else {
                    while(*p != ',') {
                        points[point_count].x *= 10;
                        points[point_count].x += (*p - '0');
                        p++;
                    }
                    p++;
                    while(*p != '\n') {
                        points[point_count].y *= 10;
                        points[point_count].y += (*p - '0');
                        p++;
                    }
                    point_count++;
                }
                break;  
            case INSTRUCTIONS:
                p += strlen("fold along ");
                if(*p == 'x') {
                    folds[fold_count].fold_direction = X;
                } else {
                    folds[fold_count].fold_direction = Y;
                }
                p += 2; // skip the x/y and '='
                folds[fold_count].value = 0;
                while(*p != '\n' && *p != '\0') {
                    folds[fold_count].value *= 10;
                    folds[fold_count].value += (*p - '0');
                    p++;
                }
                fold_count++;
                break;
        }
    }

    for(int i = 0; i < fold_count; i++) {
        fold_paper(&folds[i]);
    }

    int max_x, max_y;

    max_x = 0;
    max_y = 0;

    for(int i = 0; i < point_count; i++) {
        if(points[i].x > max_x) max_x = points[i].x;
        if(points[i].y > max_y) max_y = points[i].y;
    }

    int ** grid;
    grid = calloc(max_y + 1, sizeof(int *));
    for(int i = 0; i < max_y + 1; i++) {
        grid[i] = calloc(max_x + 1, sizeof(int));
    }

    for(int i = 0; i < point_count; i++) {
        grid[points[i].y][points[i].x] = 1;
    }

    for(int r = 0; r < max_y + 1; r++) {
        for(int c = 0; c < max_x + 1; c++) {
            printf("%s", grid[r][c] == 1 ? "█" : " ");
        }
        printf("\n");
    }
    
    free(line); 
    for(int i = 0; i < max_y + 1; i++) {
        free(grid[i]);
    }

    free(grid);

    return 0;
}

static void fold_paper(fold_t* f) {

    int to_remove[MAX_POINTS] = {-1};
    int remove_cnt = 0;
    
    for(int i = 0; i < point_count; i++) {
        if(f->fold_direction == X) {
            // if the fold direction is X, then any point with an X value bigger than f->value
            // is going to end up at f->value - (points[i].x - f->value)
            if(points[i].x > f->value) {
                int new_x = f->value - (points[i].x - f->value);

                int j = 0;
                int found = 0;
                while(j < point_count) {
                    if(points[j].x == new_x && points[j].y == points[i].y) {
                        found = 1;
                        to_remove[remove_cnt++] = i;
                        break;
                    }
                    j++;
                }
                if(!found) {
                    points[i].x = f->value - (points[i].x - f->value);
                }                
            }
        } else {
            if(points[i].y > f->value) {
                int new_y = f->value - (points[i].y - f->value);

                int j = 0;
                int found = 0;
                while(j < point_count) {
                    if(points[j].y == new_y && points[j].x == points[i].x) {
                        found = 1;
                        to_remove[remove_cnt++] = i;
                        break;
                    }
                    j++;
                }
                if(!found) {
                    points[i].y = f->value - (points[i].y - f->value);
                }
            }
        }
    }

    int point_index = 0;

    for(int i = 0; i < point_count; i++) {

        int should_be_removed = 0;
        for(int j = 0; j < remove_cnt && !should_be_removed; j++) {
            if(to_remove[j] == i) {
                should_be_removed = 1;
            }
        }
        if(!should_be_removed) {
            points[point_index++] = points[i];
        }
    }

    point_count -= remove_cnt;
}