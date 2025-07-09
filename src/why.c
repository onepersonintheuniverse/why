#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stacks.h"
typedef long long s64;

DECL_WHY_STACK_POP(s64)
DECL_WHY_STACK_PUSH(s64)

long length; // length of the file
struct stack st;

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

char *substr(char *str, int start, int end) {
    int len = end-start+1;
    char *new = malloc(len+1);
    for (char *i = str+start; i < str+end; ++i) {
        *new = *i;
        ++new;
    }
    *new -= len;
    return new;
}

/**
 * @brief the function that will do the running of the program.
 * @param prog the string which contains the program to run.
 */
void interpret(char *prog) {
    int bracketed = 0;
    int escaped = 0;
    for (; *prog; ++prog) {
        if (bracketed) {
            if (*prog == '"' || *prog == '#') bracketed = 0;
            continue;
        }
        else if (*prog == '"') {
            char *rq = match(prog, '"', '"');
            bracketed = 1;
            push(&st, 0);
            for (char *i = rq-1; i > prog; --i) {
                if (*i != '\\') push(&st, *i);
                else {
                    if (*(st.st_en-1) == 'n') *(st.st_en-1) = '\n';
                    if (*(st.st_en-1) == 't') *(st.st_en-1) = '\t';
                }
            }
        }
        else if (*prog == 'P') {
            char q;
            while (q = pop(&st)) putc(q, stdout);
        }
        else if (*prog == 'p') {
            printf("%lld", pop_s64(&st));
        }
        else if (*prog == '#') {
            char *rh = match(prog, '#', '#');
            long long x = 0;
            long long s = 1;
            for (char *i = prog+1; i < rh; ++i) {
                if (*i == '-') s = -1;
                else x = 10*x + *i - '0';
            }
            x *= s;
            push_s64(&st, x);
            bracketed = 1;
        }
        else if (*prog == '+') {
            push_s64(&st, pop_s64(&st) + pop_s64(&st));
        }
        else if (*prog == '-') {
            push_s64(&st, pop_s64(&st) - pop_s64(&st));
        }
        else if (*prog == '*') {
            push_s64(&st, pop_s64(&st) * pop_s64(&st));
        }
        else if (*prog == '/') {
            push_s64(&st, pop_s64(&st) / pop_s64(&st));
        }
    }
}

int main(int argc, char const *argv[]) {
    init_stack(&st, 4096);
    FILE *fp = fopen("./examples/helloworld.why", "r");
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
