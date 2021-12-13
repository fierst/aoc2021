#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ROWS 10
#define COLS 10

uint8_t oct[ROWS][COLS];
uint8_t flashed[ROWS][COLS];

static void reset_flashed()
{
    memset(flashed, 0, sizeof(uint8_t) * ROWS * COLS);
}

static void print_oct() {
    for(int r = 0; r < ROWS; r++) {
        for(int c = 0; c < COLS; c++) {
            printf("%d", oct[r][c]);
        }
        printf("\n");
    }
}

static bool need_to_flash()
{
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            if (oct[r][c] > 9 && !flashed[r][c])
            {
                return true;
            }
        }
    }
    return false;
}

// returns the number of flashes in each step
static int step()
{
    reset_flashed();

    int flash_count = 0;

    // First, increase count of all octopi by 1
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            oct[r][c]++;
        }
    }

    // Then, check for any octopi with a count over 9, as they will flash
    while (need_to_flash())
    {
        for (int r = 0; r < ROWS; r++)
        {
            for (int c = 0; c < COLS; c++)
            {
                if (oct[r][c] > 9 && !flashed[r][c])
                {
                    flash_count++;
                    flashed[r][c] = 1;
                    oct[r][c] = 0;
                    
                    for (int y = r - 1; y <= r + 1; y++)
                    {
                        for (int x = c - 1; x <= c + 1; x++)
                        {
                            if (x >= 0 && x < COLS && y >= 0 && y < ROWS)
                            {
                                if (!flashed[y][x])
                                {
                                    oct[y][x]++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return flash_count;
}

int main(int argc, char const *argv[])
{
    reset_flashed();

    char *line = NULL;
    size_t len;
    ssize_t read;

    int row = 0;

    while (read = getline(&line, &len, stdin) != EOF)
    {
        char *p = line;

        int col = 0;

        while (*p != '\n' && *p != EOF)
        {
            oct[row][col++] = (*p++ - '0');
        }

        row++;
    }

    free(line);


    int it = 1;
    int flash_cnt = 0;

    while(flash_cnt = step() < (ROWS * COLS)) {
        it++;
    }

    printf("%d\n", it);

    return 0;
}
