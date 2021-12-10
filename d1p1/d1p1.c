#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
    char * line;

    int cnt = 0;

    int val, prev_val = __INT32_MAX__;

    size_t len;
    ssize_t read;

    while ((read = getline(&line, &len, stdin)) != -1) {
        val = atoi(line);
        if(val > prev_val)  cnt++;
        prev_val = val;
    }

    printf("%d\n", cnt);

    return 0;
}