#ifndef _stack_h
#define _stack_h

#define STACK_DEFAULT_CAPACITY  16

typedef struct {
    int capacity;
    int top;
    char * values;
} stack_t;

void init_stack(stack_t * stack, int capacity);
void push(stack_t * stack, char val);
void pop(stack_t * stack);
char top(stack_t * stack);

#endif //_stack_h