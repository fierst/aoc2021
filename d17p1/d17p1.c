#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

int main(int argc, char const *argv[])
{
    // The y-speed at y=0 on the way down will be equal to the velocity at y=0 on the way up
    // So we need the velocity to be one lower than the lowest Y point in the problem area, as
    // the velocity will increase to the lowest Y point in the problem area after it crosses
    // Y=0. Therefore, just find the n-th triangle number for the lowest Y point (minus 1)
    // and you have the answer.
    printf("math: %d\n", (((problem_area.y1 + 1) * (problem_area.y1 + 1)) + abs((problem_area.y1 + 1)))/2);
    return 0;
}
