#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    char * line = NULL;

    size_t len;
    ssize_t read;

    int count = 0;

    while(read = getline(&line, &len, stdin) != -1) {
        char * p = line;

        while(*p != '|') {
            ++p;
        }

        ++p; // skip over '|'

        while(*p++ != '\n') {
            int char_cnt = 0;
            while(*p == ' ') {
                p++;
            }
            
            while(*p != ' ' && *p != '\n' && *p != '\0') {
                char_cnt++;
                p++;
            }

            if (char_cnt == 2 || char_cnt == 3 || char_cnt == 4 || char_cnt == 7)
                count++;
            
        }
    }

    free(line);

    printf("%d\n", count);

    return 0;
}
