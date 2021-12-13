#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

#include "graph.h"

int main(int argc, char const *argv[])
{
    char * line = NULL;
    size_t len;
    ssize_t read;

    char src[MAX_ID_LEN];
    char dest[MAX_ID_LEN];

    init_graph();
    
    while((read = getline(&line, &len, stdin)) != EOF) {
        char * p = line;
        int si=0;
        int di=0;
        
        // Read the src node id
        while(*p != '-' && si < MAX_ID_LEN) {
            src[si++] = *p;
            p++;
        }

        src[si] = '\0';
        
        // skip over '-'
        p++;

        // Read the dest node ID
        while(*p != '\0' && *p != '\n' && di < MAX_ID_LEN) {
            dest[di++] = *p;
            p++;
        }

        dest[di] = '\0';

        add_adjacency_to_graph(src, dest);
    }
    
    free(line);

    printf("%d\n", bfs());

    clean_graph();

    return 0;
}
