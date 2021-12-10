#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BINGO_WIDTH 5
#define BINGO_HEIGHT 5
#define INITIAL_CAPACITY 16

typedef struct  {
    int value;
    int selected;
} bingo_num;

typedef struct {
    bingo_num nums[BINGO_WIDTH][BINGO_HEIGHT];
    int won;
} bingo_board;

typedef struct {
    void * items;
    int capacity;
    int count;
} vector_t;

enum parse_state {
    BINGO_NUMS,
    SEPARATOR,
    BOARDS
};

typedef int (*filter_func_t)(bingo_board);

int is_empty(const char * line);
int read_number(char ** s);
void print_board(bingo_board board);
int check_board(bingo_board board);
int sum_board(bingo_board board);
int filter_won_boards(filter_func_t filter_fn, bingo_board * values, int size);
int not_won_filter(bingo_board board);

int main(int argc, char const *argv[])
{
    char * line = NULL;

    enum parse_state state = BINGO_NUMS;

    size_t len;
    ssize_t read;

    vector_t boards = {
        .capacity = INITIAL_CAPACITY,
        .count = 0,
        .items = calloc(INITIAL_CAPACITY, sizeof(bingo_board))
    };

    memset(boards.items, 0, sizeof(bingo_board) * boards.capacity);

    vector_t nums = {
        .capacity = INITIAL_CAPACITY,
        .count = 0,
        .items = calloc(INITIAL_CAPACITY, sizeof(int))
    };

    int board_index = 0;
    int row = 0;
    int col = 0;

    while(read = getline(&line, &len, stdin) != -1) {
        
        // Resize the vector if necessary
        if (boards.count >= boards.capacity) {
            boards.items = reallocarray(boards.items, boards.capacity * 2, sizeof(bingo_board));
            boards.capacity *= 2;
        }

        if(state == BINGO_NUMS) {
            char * tok;

            tok = strtok(line, ",");
            while(tok != NULL) {
                if (nums.count >= nums.capacity) {
                    nums.items = reallocarray(nums.items, nums.capacity * 2, sizeof(int));
                    nums.capacity *= 2;
                }

               ((int *)nums.items)[nums.count++] = atoi(tok);

                tok = strtok(NULL, ",");
            }

            state = SEPARATOR;
        } else {
            // Ignore newlines
            if(is_empty(line)) {
                // special case, we're looking at the separator between
                // the bingo numbers and the bingo boards 
                if(state != SEPARATOR) {
                    row = 0;
                } else {
                    // Skip to the next state
                    state = BOARDS;
                }
            } else {
                char * l = line;
                for(col = 0; col < BINGO_WIDTH; col++) {
                    ((bingo_board *)boards.items)[boards.count].nums[row][col].value = read_number(&l);                        
                }
                row++;
                if(row == BINGO_HEIGHT) boards.count++;
            }
        }

        
    }

    int last_num = -1;
    
    int done = 0;
    int n = 0; 
    int boards_left = -1;

    while(n < nums.count && !done){
        last_num = ((int *)nums.items)[n];

        for(int b = 0; b < boards.count; b++) {
            for(int r = 0; r < BINGO_HEIGHT; r++) {
                for(int c = 0; c < BINGO_HEIGHT; c++) {
                    if(((bingo_board *)boards.items)[b].nums[r][c].value == last_num)
                        ((bingo_board *)boards.items)[b].nums[r][c].selected = 1;
                }
            }

            if(check_board(((bingo_board *)boards.items)[b])) {
                ((bingo_board *)boards.items)[b].won = 1;
            }

            if(boards_left == 1) {
                if(((bingo_board *)boards.items)[b].won) {
                    printf("last_num: %d\n", last_num);
                    printf("sum: %d\n", sum_board(((bingo_board *)boards.items)[b]));
                    printf("answer: %d\n", sum_board(((bingo_board *)boards.items)[b]) * last_num);
                    done = 1;
                }
            }
        }

        boards_left = filter_won_boards(not_won_filter, ((bingo_board *)boards.items), boards.count);
        boards.count = boards_left;
        boards.items = reallocarray(boards.items, sizeof(bingo_board), boards_left);

        n++;    
    }

    // for(int i = 0; i < boards.count; i++) {
    //     printf("board %d:\n", i);
    //     print_board(((bingo_board *)boards.items)[i]);
    //     printf("\n");
    // }

    // clean-up!
    free(line);
    free(nums.items);
    free(boards.items);

    return 0;
}

int is_empty(const char * line) {
    while (*line != '\0') {
    if (!isspace((unsigned char)*line))
      return 0;
    line++;
  }
  return 1;
}

int read_number(char ** s) {
    
    int val = 0;

    char * p = *s;

    // skip all leading spaces
    while(*p == ' ') {
        ++p;
    }

    while (*p >= '0' && *p <= '9') {               /* Parse until character is valid number */
        val = val * 10 + (*p - 0x30);
        ++p;
    }

    *s = p;
    
    return val;
}

int check_board(bingo_board board) {

    // check rows
    for(int r = 0; r < BINGO_HEIGHT; r++) {
        if( board.nums[r][0].selected && 
            board.nums[r][1].selected && 
            board.nums[r][2].selected &&
            board.nums[r][3].selected && 
            board.nums[r][4].selected) {
                return 1;
            }
    }

    for(int c = 0; c < BINGO_WIDTH; c++) {
        if( board.nums[0][c].selected && 
            board.nums[1][c].selected && 
            board.nums[2][c].selected &&
            board.nums[3][c].selected && 
            board.nums[4][c].selected) {
                return 1;
            }
    }

    return 0;
}

void print_board(bingo_board board) {

    for(int r = 0; r < BINGO_HEIGHT; r++) {
        for(int c = 0; c < BINGO_WIDTH; c++) {
            if(board.nums[r][c].selected) {
                printf("\t\033[1;31m%d\033[0m", board.nums[r][c].value);
            } else {
                printf("\t%d", board.nums[r][c].value);
            }
        }
        printf("\n");
    }
}

int sum_board(bingo_board board) {
    
    int sum = 0;

    for(int r = 0; r < BINGO_HEIGHT; r++) {
        for(int c = 0; c < BINGO_WIDTH; c++) {
            if(!board.nums[r][c].selected) {
                sum += board.nums[r][c].value;
            }
        }
    }

    return sum;
}

int not_won_filter(bingo_board board) {
    return !board.won;
}

int filter_won_boards(filter_func_t filter_fn, bingo_board * values, int size) {
    int ctr = 0;

    for(int i = 0; i < size; i++) {
        if (filter_fn(values[i])) {
            memcpy((void *)&values[ctr], (void *)&values[i], sizeof(bingo_board));
            ctr++;
        }
    }

    return ctr;
}