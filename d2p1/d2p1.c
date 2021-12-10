#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INITIAL_SIZE 16

struct sub_position{
    int depth;
    int distance;
};

enum direction {
    FORWARD,
    UP,
    DOWN
};

int main(int argc, char const *argv[])
{
    struct sub_position subpos = {
        .depth = 0,
        .distance = 0
    };

    enum direction dir;

    char * line = (char *)malloc(INITIAL_SIZE * sizeof(char));

    size_t len = INITIAL_SIZE;
    ssize_t read;

    while ((read = getline(&line, &len, stdin)) != -1) {
        
        char * dat = line;

        switch(*dat) {
            case 'f': // forward
                dir = FORWARD;
                dat += strlen("forward ");
                break;
            case 'u': // up
                dir = UP;
                dat += strlen("up ");
                break;
            case 'd': // down
                dir = DOWN;
                dat += strlen("down ");
                break;
        }

        int magnitude = atoi(dat);

        switch(dir){
            case FORWARD:
                subpos.distance += magnitude;
                break;
            case UP:
                subpos.depth -= magnitude;
                break;
            case DOWN:
                subpos.depth += magnitude;
                break;
        } 
    }

    free(line);

    printf("subpos: {%d, %d}\n", subpos.distance, subpos.depth);

    /* code */
    return 0;
}
