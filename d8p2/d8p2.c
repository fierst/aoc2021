#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define NUM_SEGMENTS    7
#define NUM_DIGITS      10

//  We should be able to map digits based on whatever the 8 is
//                          gfedcba
#define ss_zero   0b0111111
#define ss_one    0b0000110
#define ss_two    0b1011011
#define ss_three  0b1001111
#define ss_four   0b1100110
#define ss_five   0b1101101
#define ss_six    0b1111101
#define ss_seven  0b0000111
#define ss_eight  0b1111111
#define ss_nine   0b1101111

//   PATTERN:
//
//    AAAA
//   F    B
//   F    B
//   F    B
//    GGGG
//   E    C
//   E    C
//   E    C
//    DDDD 
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
static void solve_for_segment_A();
static void solve_for_three();
static void solve_for_five();
static void solve_for_segment_E();
static void solve_for_segment_F();
static void solve_for_the_rest();

static void find_same_length_digit_with_one_segment_difference(digit_t * d);
static char find_segment_in_a_not_b(digit_t * a, digit_t * b);
static int segments_in_digit(digit_t * d);
static int get_index_of_digit_with_value(int v);
static int seven_seg_to_int(int ss_bits);

int main(int argc, char const *argv[])
{
    char * line = 0;
    size_t len;
    ssize_t read;

    int total_value = 0;

    while(read = getline(&line, &len, stdin) != EOF) {
        char * p = line;

        memset(digits, 0, NUM_DIGITS * sizeof(digit_t));
        memset(&segments, 0, sizeof(segment_map_t));

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

        solve_for_segment_A();
        solve_for_two();
        solve_for_three();
        solve_for_segment_E();
        solve_for_five();
        solve_for_segment_F();
        solve_for_the_rest();

        int lineval = 0;

        while(*p != '\n' && *p != '\0' && *p != EOF) {
            while(*p == ' ') {
                p++;
            }

            lineval *= 10;
            
            uint8_t bitval = 0;

            while(*p != ' ' && *p != '\n' && *p != '\0') {
                if(*p == segments.A) {
                    bitval |= 1 << 0;
                } else if(*p == segments.B) {
                    bitval |= 1 << 1;
                } else if(*p == segments.C) {
                    bitval |= 1 << 2;
                } else if(*p == segments.D) {
                    bitval |= 1 << 3;
                } else if(*p == segments.E) {
                    bitval |= 1 << 4;
                } else if(*p == segments.F) {
                    bitval |= 1 << 5;
                } else if(*p == segments.G) {
                    bitval |= 1 << 6;
                }
                p++;
            }

            lineval += seven_seg_to_int(bitval);
        }

        total_value += lineval;

    }

    free(line);

    printf("total: %d\n", total_value);

    return 0;
}

static void solve_for_two() {
    char * one_segments = digits[get_index_of_digit_with_value(1)].raw;

    // figure out which one is the B segment
    for(int i = 0; i < 2 && segments.B == '\0'; i++) {
        
        int sc = 0; // segment count (how many times this segment is used)

        for(int d = 0; d < NUM_DIGITS; d++)  {
            int ri = 0;
            while(digits[d].raw[ri] != '\0' && ri < NUM_SEGMENTS) {
                if(digits[d].raw[ri] == one_segments[i])    sc++;
                ri++;
            }
        }

        if(sc == (NUM_DIGITS - 1)) {
            segments.C = one_segments[i];
            segments.B = one_segments[!i]; // idk if this will always work, but it looks clean
            break;
        }
    }

    int two_index = -1;

    // find the digit without a b segment
    for(int d = 0; d < NUM_DIGITS; d++) {
        int ri = 0; // raw index
        int ft = 1; // found two
        
        while(digits[d].raw[ri] != '\0' && ri < NUM_SEGMENTS) {
            if(digits[d].raw[ri++] == segments.C) ft = 0;
        }

        if(ft) {
            digits[d].solved = 1;
            digits[d].value = 2;
            break;
        }        
    }
}

static void solve_for_segment_A() {
    digit_t * ones_digit = &digits[get_index_of_digit_with_value(1)];
    digit_t * sevens_digit = &digits[get_index_of_digit_with_value(7)];

    segments.A = find_segment_in_a_not_b(sevens_digit, ones_digit);
}

static void solve_for_segment_E() {
    digit_t * twos_digit = &digits[get_index_of_digit_with_value(2)];
    digit_t * threes_digit = &digits[get_index_of_digit_with_value(3)];

    segments.E = find_segment_in_a_not_b(twos_digit, threes_digit);
}

static void solve_for_segment_F() {
    digit_t * fives_digit = &digits[get_index_of_digit_with_value(5)];
    digit_t * threes_digit = &digits[get_index_of_digit_with_value(3)];

    segments.F = find_segment_in_a_not_b(fives_digit, threes_digit);
}

static void solve_for_three() {
    digit_t * twos_digit = &digits[get_index_of_digit_with_value(2)];

    find_same_length_digit_with_one_segment_difference(twos_digit);
}

static void solve_for_five() {
    digit_t * threes_digit = &digits[get_index_of_digit_with_value(3)];

    find_same_length_digit_with_one_segment_difference(threes_digit);
}

static void solve_for_the_rest() {
    digit_t * fours_digit = &digits[get_index_of_digit_with_value(4)];

    for(int d = 0; d < NUM_DIGITS; d++) {
        if(!digits[d].solved) {
            char diffc = find_segment_in_a_not_b(fours_digit, &digits[d]);
            // if digits[d] = 0 then diffc should be segment G (which we don't know)
            // if digits[d] = 6 then diffc should be segment B (which we do know)
            // if digits[d] = 9 then diffc should be NULL

            int new_value = -1;

            if(diffc == segments.B) {
                new_value = 6;
            } else if (diffc == '\0') {
                new_value = 9;
            } else {
                new_value = 0;
                segments.G = diffc;
            }
            
            digits[d].solved = 1;
            digits[d].value = new_value;
        }
    }

    digit_t a_digit_placeholder = {
        .solved = 0,
        .value = 'a'
    };


    int ri = 0;
    a_digit_placeholder.raw[ri++] = segments.A;
    a_digit_placeholder.raw[ri++] = segments.B;
    a_digit_placeholder.raw[ri++] = segments.C;
    a_digit_placeholder.raw[ri++] = segments.E;
    a_digit_placeholder.raw[ri++] = segments.F;
    a_digit_placeholder.raw[ri++] = segments.G;

    segments.D = find_segment_in_a_not_b(&digits[get_index_of_digit_with_value(8)], &a_digit_placeholder);
}

static void find_same_length_digit_with_one_segment_difference(digit_t * d) {
    // segment count
    int sc = segments_in_digit(d);

    // Try to find a digit that's the same number
    // of segments as the given digit but differs
    // by one segment
    for(int di = 0; di < NUM_DIGITS; di++) {
        if(!digits[di].solved && segments_in_digit(&digits[di]) == sc) {
            int diff = sc; // differing chars
            for(int ti = 0; ti < sc; ti++) {
                for(int td = 0; td < sc; td++) {
                    if(digits[di].raw[td] == d->raw[ti]) diff--;
                }
            }
            if(diff == 1) {
                digits[di].solved = 1;
                int new_value = 0;

                switch(d->value) {
                    case 2:
                        new_value = 3;
                        break;
                    case 3:
                        new_value = 5;
                        break;
                }
                digits[di].value = new_value;
                break;
            }
        }
    }
}

static char find_segment_in_a_not_b(digit_t * a, digit_t * b) {
    int sa = segments_in_digit(a);
    int sb = segments_in_digit(b);

    char diff_char = '\0';

    for(int i = 0; i < sa; i++) {
        int p = 0; // present
        for(int j = 0; j < sb; j++) {
            if(a->raw[i] == b->raw[j])  p = 1;
        }
        if(!p) {
            diff_char = a->raw[i];
            break;
        }
    }    

    return diff_char;
}

static int segments_in_digit(digit_t * d) {
    int s = 0;

    while(s < NUM_SEGMENTS && d->raw[s] != '\0')    s++;

    return s;
}

static int get_index_of_digit_with_value(int v) {
    int i = -1;

    for(int d = 0; d < NUM_DIGITS; d++) {
        if(digits[d].solved && digits[d].value == v) {
            i = d;
            break;
        }
    }

    return i;
}

static int seven_seg_to_int(int ss_bits) {
    switch(ss_bits) {
        case ss_zero: return 0;
        case ss_one: return 1;
        case ss_two: return 2;
        case ss_three: return 3;
        case ss_four: return 4;
        case ss_five: return 5;
        case ss_six: return 6;
        case ss_seven: return 7;
        case ss_eight: return 8;
        case ss_nine: return 9;
        default: return -1;
    }
}