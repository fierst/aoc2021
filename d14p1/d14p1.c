#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define SEQ_LENGTH          2
#define PRECURSOR_LENGTH    32
#define RULES_LENGTH        128

enum parse_state {
    PRECURSOR,
    RULES
};

typedef struct {
    char seq[SEQ_LENGTH];
    char sub;
} rule_t;

rule_t rules[RULES_LENGTH] = {0};
int rule_count = 0;

typedef struct {
    char* items;
    int count;
    int capacity;
} vector_t;

typedef struct {
    char c;
    uint32_t count;
} freq_cnt_t;

vector_t polymer_string;
char * polymer_copy;

static void push_char(char to_push) {
    polymer_string.items[polymer_string.count++] = to_push;

    if(polymer_string.count == polymer_string.capacity) {
        polymer_string.capacity *= 2;
        polymer_string.items = reallocarray(polymer_string.items, polymer_string.capacity, sizeof(char));
    }
}

static char get_sub_from_rule(const char * to_check) {
    char ret = 0;

    for(int i = 0; i < rule_count; i++) {
        if(strncmp(rules[i].seq, to_check, SEQ_LENGTH) == 0) {
            ret = rules[i].sub;
            break;
        }
    }

    return ret;
}

static void apply_rules(void) {
    // Copy over the polymer string to the copy
    polymer_copy = calloc(polymer_string.count, sizeof(char));
    memcpy(polymer_copy, polymer_string.items, polymer_string.count);

    int len = polymer_string.count;

    // Reset polymer_string.count so we push to the beginning
    polymer_string.count = 0;

    char *p;

    for(p = polymer_copy; p < (polymer_copy + len - 1); p++) {
        push_char(*p);
        char s = get_sub_from_rule((const char *)p);
        if(s != 0)  push_char(s);
    }
    push_char(*p);

    free(polymer_copy);
}

int main(int argc, char const *argv[])
{
    enum parse_state state = PRECURSOR;
    char * line = NULL;
    size_t len;
    ssize_t read;


    polymer_string.count = 0;
    polymer_string.capacity = PRECURSOR_LENGTH;
    polymer_string.items = calloc(PRECURSOR_LENGTH, sizeof(char));
    
    while(read = getline(&line, &len, stdin) != EOF) {
        char *p = line;

        switch(state) {
            case PRECURSOR:
                if(strlen(p) == 1) {
                    state = RULES;
                } else {
                    while(*p != '\n') {
                        push_char(*p);
                        p++;
                    }
                }
                break;
            case RULES:
                sprintf(rules[rule_count].seq, "%.2s", p);
                p += 6;
                rules[rule_count].sub = *p;
                rule_count++;
                break;
        }

    }

    for(int i = 0; i < 10; i++) {
        apply_rules();
    }

    freq_cnt_t * freq_cnt = calloc(PRECURSOR_LENGTH, sizeof(freq_cnt_t));
    int freq_cnt_capacity = PRECURSOR_LENGTH;
    int freq_cnt_count = 0;

    for(char* p = polymer_string.items; p < (polymer_string.items + polymer_string.count); p++) {
        bool found = false;
        
        for(int i = 0; i < freq_cnt_count; i++) {
            if(freq_cnt[i].c == *p) {
                freq_cnt[i].count++;
                found = true;
            }
        } 

        if(!found) {
            freq_cnt[freq_cnt_count].c = *p;
            freq_cnt[freq_cnt_count].count = 1;
            freq_cnt_count++;
            if(freq_cnt_count == freq_cnt_capacity) {
                freq_cnt_capacity *= 2;
                freq_cnt = reallocarray(freq_cnt, freq_cnt_capacity, sizeof(freq_cnt_t));
            }
        }
    }

    int max = 0;
    int min = __INT_MAX__;

    for(int i = 0; i < freq_cnt_count; i++) {
        if(freq_cnt[i].count > max) {
            max = freq_cnt[i].count;
        }

        if(freq_cnt[i].count < min) {
            min = freq_cnt[i].count;
        }
    }

    printf("%d\n", max - min);
    
    free(line);
    free(polymer_string.items);
    return 0;
}
