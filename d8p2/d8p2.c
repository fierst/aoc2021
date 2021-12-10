#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define NUM_SEGMENTS    7
#define NUM_DIGITS      10

// //  We should be able to map digits based on whatever the 8 is
// //                          gfedcba
// const uint8_t ss_zero   = 0b0111111;
// const uint8_t ss_one    = 0b0000110;
// const uint8_t ss_two    = 0b1011011;
// const uint8_t ss_three  = 0b1001111;
// const uint8_t ss_four   = 0b1100110;
// const uint8_t ss_five   = 0b1101101;
// const uint8_t ss_six    = 0b1111101;
// const uint8_t ss_seven  = 0b0000111;
// const uint8_t ss_eight  = 0b1111111;
// const uint8_t ss_nine   = 0b1101111;
typedef struct {
    char A;
    char B;
    char C;
    char D;
    char E;
    char F;
    char G;
} segment_map_t;

typedef struct {
    uint8_t solved;
    uint8_t value;
    char raw[NUM_SEGMENTS];
} digit_t;

digit_t digits[NUM_DIGITS];

segment_map_t segments = {0};

static void solve_for_two();
static void solve_for_three();
static void solve_for_five();
static void solve_for_zero();

int main(int argc, char const *argv[])
{
    char * line = 0;
    size_t len;
    ssize_t read;

    while(read = getline(&line, &len, stdin) != EOF) {
        char * p = line;

        int di = 0;

        // First fill in the digits array
        while(*p != '|') {
            // read each digit

            // Raw index, for indexing digit[di].raw
            int ri = 0;

            while(*p != ' ') {
                digits[di].raw[ri++] = *p;
                p++;
            }

            if(ri == 2) {
                digits[di].solved = 1;
                digits[di].value = 1;
            } else if (ri == 3) {
                digits[di].solved = 1;
                digits[di].value = 7;
            } else if (ri == 4) {
                digits[di].solved = 1;
                digits[di].value = 4;
            } else if (ri == 7) {
                digits[di].solved = 1;
                digits[di].value = 8;
            }

            p++;
            di++;
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
        }

    }

    free(line);

    return 0;
}

static void solve_for_two() {
    char one_segments[2];
    
    // map the one segments
    for(int d = 0; d < NUM_DIGITS; d++) {
        if(digits[d].solved && digits[d].value == 1) {
            strncpy(one_segments, digits[d].raw, 2);
        }
    }

    // figure out which one is the B segment
    for(int i = 0; i < 2 && segments.B == NULL; i++) {
        
        int sc = 0;

        for(int d = 0; d < NUM_DIGITS; d++)  {
            int ri = 0;
            while(digits[d].raw[ri] != NULL && ri < NUM_SEGMENTS) {
                if(digits[d].raw[ri] == one_segments[i])    sc++;
                ri++;
            }
        }

        if(sc == 1) {
            segments.B = one_segments[i];
        }
    }

    int two_index = -1;

    // find the digit without a b segment
    for(int d = 0; d < NUM_DIGITS; d++) {
        int ri = 0;
        while(digits[d].raw[ri] != NULL && ri < NUM_SEGMENTS) {

        }
    }

}

static void solve_for_three() {
    
}

static void solve_for_five() {

}

static void solve_for_zero() {

}