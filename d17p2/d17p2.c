#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef struct {
    int x1;
    int x2;
    int y1;
    int y2;
} area_t;

area_t test_area = {
    .x1 = 20,
    .x2 = 30,
    .y1 = -10,
    .y2 = -5
};

area_t problem_area = {
    .x1 = 206,
    .x2 = 250,
    .y1 = -105,
    .y2 = -57
};

int get_number_of_valid_points(area_t * area) {
    int min_x_velocity = ceil((-1 + sqrt(1.0 + (8.0 * (area->x1)))) / 2.0f);
    int max_x_velocity = area->x2;
    int min_y_velocity = area->y1;
    int max_y_velocity = abs(area->y1) - 1;

    int valid_points = 0;

    for (int xv = min_x_velocity; xv <= max_x_velocity; xv++) {
        for(int yv = min_y_velocity; yv <= max_y_velocity; yv++) {
            
            int x = 0;
            int y = 0;
            int x_velocity = xv;
            int y_velocity = yv;

            int point_ok = 0;

            for(int n = 0; n < (max_y_velocity * 2) + 5 && !point_ok; n++) {
                x += x_velocity--;
                if(x_velocity < 0) x_velocity = 0;
                y += y_velocity--;
                if(x >= area->x1 && x <= area->x2 && y >= area->y1 && y <= area->y2) {
                    valid_points++;
                    printf("[%d,%d]\n", xv, yv);
                    point_ok = 1;
                }
            }
        }
    }

    return valid_points;
}

int main(int argc, char const *argv[])
{
    //printf("%d\n", get_number_of_valid_points(&test_area));
    printf("%d\n", get_number_of_valid_points(&problem_area));
    return 0;
}
