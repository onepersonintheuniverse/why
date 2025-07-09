#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STACK 4096

char stack[STACK] = {}; // basic "stack" with 4096 bytes
char *stack_end = stack; // points to the element after the stack's top
long length; // length of the file
void push(char x) { // push to the stack
    if (stack_end == stack + STACK) return;
    *(stack_end) = x;
    ++stack_end;
}

char pop() { // pop from the stack and return the popped element
    if (stack_end == stack) return 0;
    --stack_end;
    return *stack_end;
}

/**
 * @brief bracket/quote matching function. match first instance of `lb` with an instance
 * @brief of `rb` and return pointer to said instance of `rb`.
 * @param str the string where you'll match the bracket.
 * @param lb left bracket character
 * @param rb right bracket character
 */
char *match(char *str, char lb, char rb) {
    int score = 0;
    int brackets = 0;
    for (; *str; ++str) {
        if (lb == rb && *str == lb) ++score;
        else if (*str == lb) ++score;
        else if (*str == rb) --score;
        if (*str == lb || *str == rb) brackets++;
        if (lb == rb && score == 2) return str;
        else if (!score && brackets) return str;
    }
    return str;
}

/**
 * @brief the function that will do the running of the program.
 * @param prog the string which contains the program to run.
 */
void interpret(char *prog) {
    int bracketed = 0;
    for (; *prog; ++prog) {
        if (bracketed) {
            if (*prog == '"') bracketed = 0;
            continue;
        }
        else if (*prog == '"') {
            char *rq = match(prog, '"', '"');
            bracketed = 1;
            push(0);
            for (char *i = rq-1; i > prog; --i) push(*i);
        }
        else if (*prog == 'P') {
            char q;
            while (q = pop()) putc(q, stdout);
        }
    }
}

int main(int argc, char const *argv[]) {
    FILE *fp = fopen("helloworld.why", "r");
    // find how long the file is and dump it all in a char *
    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buffer = malloc(length+1);
    *(buffer+length) = 0;
    fread(buffer, 1, length, fp);
    interpret(buffer);
    return 0;
}
