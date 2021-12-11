#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "stack.h"

#define INITIAL_CAPACITY 128

#define STACK_AUTOCOMPLETE_PARENTHESES      1
#define STACK_AUTOCOMPLETE_SQUARE_BRACKET   2
#define STACK_AUTOCOMPLETE_CURLY_BRACE      3
#define STACK_AUTOCOMPLETE_CARET            4

typedef struct {
    uint64_t* items;
    int count;
    int capacity;
} vector_t;

void vector_push_back(vector_t * v, uint64_t value);

int vector_sort_comp(const void * a, const void * b);

int main(int argc, char const *argv[])
{
    char * line = NULL;
    size_t len;
    ssize_t read;

    vector_t stack_scores = {
        .capacity = INITIAL_CAPACITY,
        .count = 0,
        .items = calloc(INITIAL_CAPACITY, sizeof(int))
    };

    stack_t stack;

    uint8_t stack_error;

    uint64_t stack_score;
    
    while(read = getline(&line, &len, stdin) != EOF) {

        stack_score = 0;

        char * p = line;

        init_stack(&stack, STACK_DEFAULT_CAPACITY);

        stack_error = 0;

        while(*p != '\n' && *p != '\0' && *p != EOF && !stack_error) {
            switch(*p) {
                case '{':
                case '<':
                case '[':
                case '(':
                    push(&stack, *p);
                    break;
                case '}':
                    if(top(&stack) == '{') {
                        pop(&stack);
                    } else {
                        stack_error = 1;
                    }
                    break;
                case '>':
                    if(top(&stack) == '<') {
                        pop(&stack);
                    } else {
                        stack_error = 1;
                    }
                    break;
                case ']':
                    if(top(&stack) == '[') {
                        pop(&stack);
                    } else {
                        stack_error = 1;
                    }
                    break;
                case ')':
                    if(top(&stack) == '(') {
                        pop(&stack);
                    } else {
                        stack_error = 1;
                    }
                    break;
            }  

            p++;           
        }

        if(!stack_error) {
            while(stack.top != 0) {                
                stack_score *= 5;
                switch(top(&stack)) {
                    case '{':
                        stack_score += STACK_AUTOCOMPLETE_CURLY_BRACE;
                        break;
                    case '<':
                        stack_score += STACK_AUTOCOMPLETE_CARET;
                        break;
                    case '[':
                        stack_score += STACK_AUTOCOMPLETE_SQUARE_BRACKET;
                        break;
                    case '(':
                        stack_score += STACK_AUTOCOMPLETE_PARENTHESES;
                        break;
                }
                pop(&stack);
            }

            vector_push_back(&stack_scores, stack_score);
        }
    }

    qsort(stack_scores.items, stack_scores.count, sizeof(uint64_t), vector_sort_comp);

    printf("%ld\n", stack_scores.items[stack_scores.count / 2]);
    
    free(line);    
    free(stack.values);
    free(stack_scores.items);

    return 0;
}

void vector_push_back(vector_t * v, uint64_t value) {
    v->items[v->count++] = value;
    if(v->count == v-> capacity) {
        v->capacity *= 2;
        v->items = reallocarray(v->items, v->capacity, sizeof(uint64_t));
    }
}   

int vector_sort_comp(const void * a, const void * b) {
    if (*(uint64_t *)a > *(uint64_t *)b) {
        return 1;
    } else if (*(uint64_t *)a < *(uint64_t *)b) {
        return -1;
    } else {
        return 0;
    }
    
}
