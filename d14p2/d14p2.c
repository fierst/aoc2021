#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define SEQ_LENGTH 2
#define PRECURSOR_LENGTH 32
#define RULES_LENGTH 128

enum parse_state
{
    PRECURSOR,
    RULES
};

typedef struct
{
    char seq[SEQ_LENGTH];
    char sub;
} rule_t;

rule_t rules[RULES_LENGTH] = {0};
int rule_count = 0;

typedef struct
{
    char c;
    uint64_t count;
} char_freq_t;

typedef struct
{
    char c[2];
    uint64_t count;
} char_pair_t;

char_freq_t *char_freq;
int char_freq_count;
int char_freq_capacity = PRECURSOR_LENGTH;

char_pair_t *char_pairs;
int char_pair_count;
int char_pair_capacity = PRECURSOR_LENGTH;

static int find_char_pair(const char *to_find);
static void inc_char_pair(const char *to_add, uint64_t how_many);
static void dec_char_pair(const char *to_remove, uint64_t how_many);

static int find_char(const char to_find);
static void inc_char(const char to_add, uint64_t how_many);

static int char_freq_sort_func(const void * a, const void * b) {
    if(((char_freq_t *)a)->count > ((char_freq_t *)b)->count) {
        return 1;
    } else if (((char_freq_t *)a)->count < ((char_freq_t *)b)->count) {
        return -1;
    } else {
        return 0;
    }
}

static char get_sub_from_rule(const char *to_check)
{
    char ret = 0;

    for (int i = 0; i < rule_count; i++)
    {
        if (strncmp(rules[i].seq, to_check, SEQ_LENGTH) == 0)
        {
            ret = rules[i].sub;
            break;
        }
    }

    return ret;
}

static void apply_rules(void)
{

    // copy existing char pairs
    char_pair_t *pairs_copy = calloc(char_pair_count, sizeof(char_pair_t));
    memcpy(pairs_copy, char_pairs, char_pair_count * sizeof(char_pair_t));
    int copy_count = char_pair_count;

    // Iterate all char pairs (in the copy so we don't mess up the main array)
    for (int i = 0; i < copy_count; i++)
    {

        if (char_pairs[i].count > 0)
        {
            char np1[2], np2[2];
            char sc = get_sub_from_rule(pairs_copy[i].c);

            if (sc != 0)
            {
                // For each char pair, we need to:

                // Subtract [count] from char_pairs[i]
                dec_char_pair(pairs_copy[i].c, pairs_copy[i].count);

                // Add [count] chars to char_freq
                inc_char(sc, pairs_copy[i].count);

                // Add [count] [np1] and [np2] to char_pairs
                np1[0] = pairs_copy[i].c[0];
                np1[1] = sc;

                np2[0] = sc;
                np2[1] = pairs_copy[i].c[1];

                inc_char_pair((const char *)np1, pairs_copy[i].count);
                inc_char_pair((const char *)np2, pairs_copy[i].count);
            }
        }
    }

    free(pairs_copy);
}

int main(int argc, char const *argv[])
{
    enum parse_state state = PRECURSOR;

    char_freq_count = 0;
    char_freq = calloc(char_freq_capacity, sizeof(char_freq_t));

    char_pair_count = 0;
    char_pairs = calloc(char_pair_capacity, sizeof(char_pair_t));

    char *line = NULL;
    size_t len;
    ssize_t read;

    while (read = getline(&line, &len, stdin) != EOF)
    {
        char *p = line;

        switch (state)
        {
        case PRECURSOR:
            if (strlen(p) == 1)
            {
                state = RULES;
            }
            else
            {
                int l = strlen(p);
                while (*p != '\n')
                {
                    inc_char(*p, 1);
                    if (p - line < l - 2) // last character plus \n
                        inc_char_pair(p, 1);
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

    for (int i = 0; i < 40; i++)
    {
        apply_rules();
    }

    qsort(char_freq, char_freq_count, sizeof(char_freq_t), char_freq_sort_func);

    printf("%ld\n", char_freq[char_freq_count - 1].count - char_freq[0].count);

    free(line);
    free(char_freq);
    free(char_pairs);
    return 0;
}

static int find_char_pair(const char *to_find)
{
    int ret = -1;
    for (int i = 0; i < char_pair_count; i++)
    {
        if (strncmp(to_find, char_pairs[i].c, 2) == 0)
        {
            ret = i;
            break;
        }
    }
    return ret;
}

static void inc_char_pair(const char *to_add, uint64_t how_many)
{
    int cpi = find_char_pair(to_add);
    if (cpi == -1)
    {
        strncpy(char_pairs[char_pair_count].c, to_add, 2);
        char_pairs[char_pair_count].count += how_many;
        char_pair_count++;
        if (char_pair_count == char_pair_capacity)
        {
            char_pair_capacity *= 2;
            char_pairs = reallocarray(char_pairs, char_pair_capacity, sizeof(char_pair_t));
        }
    }
    else
    {
        char_pairs[cpi].count += how_many;
    }
}

static void dec_char_pair(const char *to_remove, uint64_t how_many)
{
    int cpi = find_char_pair(to_remove);
    if (cpi == -1)
    {
        return;
    }
    else
    {
        char_pairs[cpi].count -= how_many;
    }
}

static int find_char(const char to_find)
{
    int ret = -1;
    for (int i = 0; i < char_freq_count; i++)
    {
        if (char_freq[i].c == to_find)
        {
            ret = i;
            break;
        }
    }
    return ret;
}

static void inc_char(const char to_add, uint64_t how_many)
{
    int ci = find_char(to_add);
    if (ci == -1)
    {
        char_freq[char_freq_count].c = to_add;
        char_freq[char_freq_count].count += how_many;
        char_freq_count++;
        if (char_freq_count == char_freq_capacity)
        {
            char_freq_capacity *= 2;
            char_freq = reallocarray(char_freq, char_freq_capacity, sizeof(char_freq_t));
        }
    }
    else
    {
        char_freq[ci].count += how_many;
    }
}
