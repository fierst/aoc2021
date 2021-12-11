#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "stack.h"

#define STACK_ERROR_PARENTHESES     3
#define STACK_ERROR_SQUARE_BRACKET  57
#define STACK_ERROR_CURLY_BRACE     1197
#define STACK_ERROR_CARET           25137

int main(int argc, char const *argv[])
{
    char * line = NULL;
    size_t len;
    ssize_t read;

    stack_t stack;

    uint8_t stack_error;

    int stack_error_value = 0;
    
    while(read = getline(&line, &len, stdin) != EOF) {
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
                        stack_error_value += STACK_ERROR_CURLY_BRACE;
                    }
                    break;
                case '>':
                    if(top(&stack) == '<') {
                        pop(&stack);
                    } else {
                        stack_error = 1;
                        stack_error_value += STACK_ERROR_CARET;
                    }
                    break;
                case ']':
                    if(top(&stack) == '[') {
                        pop(&stack);
                    } else {
                        stack_error = 1;
                        stack_error_value += STACK_ERROR_SQUARE_BRACKET;
                    }
                    break;
                case ')':
                    if(top(&stack) == '(') {
                        pop(&stack);
                    } else {
                        stack_error = 1;
                        stack_error_value += STACK_ERROR_PARENTHESES;
                    }
                    break;
            }

            if(!stack_error)    p++;                
        }
    }
    
    free(line);    

    free(stack.values);

    printf("%d\n", stack_error_value);

    return 0;
}
