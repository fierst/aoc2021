#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "pointqueue.h"

uint32_t grid[500][500];
uint32_t dist[500][500];

point_queue_t pq;

int col_count = 0;
int row_count = 0;

static int djikstra();

/**
 * @brief Get the neighbors for a given point
 * 
 * @param point the point to get neighbors for
 * @param p a pointer to an array that holds the resultant neighbors
 * @return int number of neighbors returned
 */
static int get_neighbors(point_t point, point_t * p);

int main(int argc, char const *argv[])
{
    memset(dist, UINT32_MAX, sizeof(uint32_t)*500*500);
    char * line = NULL;
    size_t len;
    ssize_t read;

    init_point_queue(&pq);

    int r = 0;
    int c = 0;
  
    // build the grid
    while(read = getline(&line, &len, stdin) != EOF) {
        char *p = line;

        c = 0;
        
        while(*p != '\n' && *p != '\0') {
            grid[r][c++] = (*p - '0');
            p++;
        }

        r++;
    }

    row_count = r;
    col_count = c;

    // lol this is awful
    for(int i = 0; i < 8; i++) {
        for(int r = 0; r < row_count; r++) {
            for(int c = 0; c < col_count; c++) {
                int new_num = grid[r][c] + (i + 1);
                if(new_num > 9) {
                    new_num = new_num % 9;
                }
                switch(i) {
                    case 0:
                    {
                        grid[r][c+col_count] = new_num;
                        grid[r+row_count][c] = new_num;
                    }
                    break;
                    case 1:
                    {
                        grid[r][c+(2*col_count)] = new_num;
                        grid[r+(2*row_count)][c] = new_num;
                        grid[r+row_count][c+col_count] = new_num;
                    }
                    break;
                    case 2:
                    {
                        grid[r][c+(3*col_count)] = new_num;
                        grid[r+(3*row_count)][c] = new_num;
                        grid[r+row_count][c+(2*col_count)] = new_num;
                        grid[r+(2*row_count)][c+col_count] = new_num;
                    }
                    break;
                    case 3: 
                    {
                        grid[r][c+(4*col_count)] = new_num;
                        grid[r+(4*row_count)][c] = new_num;
                        grid[r+row_count][c+(3*col_count)] = new_num;
                        grid[r+(3*row_count)][c+col_count] = new_num;
                        grid[r+(2*row_count)][c+(2*col_count)] = new_num;
                    }
                    break;
                    case 4:
                    {
                        grid[r+row_count][c+(4*col_count)] = new_num;
                        grid[r+(4*row_count)][c+col_count] = new_num;
                        grid[r+(2*row_count)][c+(3*col_count)] = new_num;
                        grid[r+(3*row_count)][c+(2*col_count)] = new_num;
                    }
                    break;
                    case 5:
                    {
                        grid[r+(2*row_count)][c+(4*col_count)] = new_num;
                        grid[r+(4*row_count)][c+(2*col_count)] = new_num;
                        grid[r+(3*row_count)][c+(3*col_count)] = new_num;
                    }
                    break;
                    case 6:
                    {
                        grid[r+(3*row_count)][c+(4*col_count)] = new_num;
                        grid[r+(4*row_count)][c+(3*col_count)] = new_num;
                    }
                    break;
                    case 7: 
                    {
                        grid[r+(4*row_count)][c+(4*col_count)] = new_num;
                    }
                    break;
                }
                
            }
        }
    }

    row_count *= 5;
    col_count *= 5;

    free(line);

    djikstra();

    printf("%d\n", dist[row_count-1][col_count-1]);

    return 0;
}

static int djikstra() {
    
    dist[0][0] = 0;

    point_t init = {
        .r = 0,
        .c = 0, 
        .dist = 0
    };

    pq.insert(&pq, init);

    point_t neighbors[4];

    while(pq.count != 0) {
        point_t to_check = pq.next(&pq);

        int nn = get_neighbors(to_check, neighbors);

        for(int i = 0; i < nn; i++) {
            int nr = neighbors[i].r;
            int nc = neighbors[i].c;

            // get dist val for each neighbor
            neighbors[i].dist = to_check.dist + grid[nr][nc];

            // update dist (if needed)
            if(neighbors[i].dist < dist[nr][nc]) {
                dist[nr][nc] = neighbors[i].dist;
                // put each neighbor in pq
                pq.insert(&pq, neighbors[i]);
            }
        }
            

    }

}

static int get_neighbors(point_t point, point_t * p) {
    
    int npc = 0; // new point count

    int pr = point.r;
    int pc = point.c;

    point_t newpt = {
        .r = pr,
        .c = pc,
        .dist = __UINT32_MAX__
    };

    // down
    if(pr + 1 < row_count) {
        newpt.r = pr + 1;
        newpt.c = pc;
        *p++ = newpt;
        npc++;
    }

    // right
    if(pc + 1 < col_count) {
        newpt.r = pr;
        newpt.c = pc + 1;
        *p++ = newpt;
        npc++;
    }

    // up
    if(pr - 1 >= 0) {
        newpt.r = pr - 1;
        newpt.c = pc;
        *p++ = newpt;
        npc++;
    }
    
    // left
    if(pc - 1 >= 0) {
        newpt.r = pr;
        newpt.c = pc - 1;
        *p++ = newpt;
        npc++;
    }

    return npc;
}