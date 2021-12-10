#include <stdio.h>
#include <stdlib.h>

#define INITIAL_SIZE 16

#define ARRAY_CAPACITY(x)   sizeof(x)/sizeof(x[0])

typedef struct {
    int * vals;
    int capacity;
    int count;
} vector;

int main(int argc, char const *argv[])
{
    vector nums;
    nums.capacity = INITIAL_SIZE;
    nums.count = 0;
    nums.vals = malloc(sizeof(int) * INITIAL_SIZE);

    char * line = NULL;
    size_t len;
    ssize_t read;

    while ((read = getline(&line, &len, stdin)) != -1) {
        if(nums.count >= nums.capacity) {
            nums.vals = (int *)reallocarray(nums.vals, nums.capacity * 2, sizeof(int));
            nums.capacity *= 2;
        }

        nums.vals[nums.count++] = atoi(line);
    }

    int prev_sum = __INT32_MAX__;
    int new_sum = 0;
    int inc_cnt = 0;

    for(int i = 1; i < nums.count - 1; i++) {
        new_sum = nums.vals[i - 1] + nums.vals[i] + nums.vals[i + 1];
        if (new_sum > prev_sum) {
            inc_cnt ++;
        }
        prev_sum = new_sum;
    }

    free(nums.vals);
		free(line);

    printf("inc_cnt = %d\n", inc_cnt);

    return 0;
}
