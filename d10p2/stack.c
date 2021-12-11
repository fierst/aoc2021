#include "stack.h"

#include <stdlib.h>

void init_stack(stack_t * stack, int capacity) {
    stack->capacity = capacity;
    stack->top = 0;
    stack->values = calloc(capacity, sizeof(char)); 
}

void push(stack_t * stack, char val) {
    stack->values[stack->top++] = val;
    if(stack->top == stack->capacity) {
        stack->capacity *= 2;
        stack->values = reallocarray(stack->values, stack->capacity, sizeof(char));
    }
}

void pop(stack_t * stack) {
    stack->values[stack->top--] = '\0';
}

char top(stack_t * stack) {
    return stack->values[stack->top - 1];
}