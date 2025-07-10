#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stacks.h>
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

char *substr(char *start, char *end) {
    int len = end-start+1;
    char *new = malloc(len+1);
    *(new+len) = 0;
    strncpy(new, start, len);
    return new;
}

/**
 * @brief the function that will do the running of the program.
 * @param prog the string which contains the program to run.
 */
void interpret(char *prog) {
    int bracketed[4] = {};
    char *p = prog;
    for (; *prog; ++prog) {
        if (bracketed[0] || bracketed[1] || bracketed[2] || bracketed[3]) {
            if (*prog == '"' && bracketed[0]) --bracketed[0];
            if (*prog == '#' && bracketed[1]) --bracketed[1];
            if (*prog == ']' && bracketed[2]) --bracketed[2];
            if (*prog == ')' && bracketed[3]) --bracketed[3];
            continue;
        }
        else if (*prog == '"') {
            char *rq = match(prog, '"', '"');
            ++bracketed[0];
            push(&st, 0);
            for (char *i = rq-1; i > prog; --i) {
                if (*i != '\\') push(&st, *i);
                else {
                    if (*(st.st_en-1) == 'n') *(st.st_en-1) = '\n';
                    if (*(st.st_en-1) == 't') *(st.st_en-1) = '\t';
                    if (*(st.st_en-1) == '\\') *(st.st_en-1) = '\\';
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
        else if (*prog == 'g') {
            s64 x; scanf("%lld", &x);
            push_s64(&st, x);
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
            ++bracketed[1];
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
        else if (*prog == '%') {
            push_s64(&st, pop_s64(&st) % pop_s64(&st));
        }
        else if (*prog == '[') {
            char *rb = match(prog, '[', ']');
            char *q = substr(prog+1, rb-1);
            while (pop_s64(&st)) interpret(q);
            ++bracketed[2];
        }
        else if (*prog == '(') {
            char *rb = match(prog, '(', ')');
            char *q = substr(prog+1, rb-1);
            s64 m = pop_s64(&st);
            for (s64 i = 0; i < m; ++i)
                interpret(q);
            ++bracketed[3];
        }
        else if (*prog == '<') {
            push_s64(&st, pop_s64(&st) < pop_s64(&st));
        }
        else if (*prog == '>') {
            push_s64(&st, pop_s64(&st) > pop_s64(&st));
        }
        else if (*prog == '=') {
            push_s64(&st, pop_s64(&st) == pop_s64(&st));
        }
        else if (*prog == '&') {
            push_s64(&st, pop_s64(&st) && pop_s64(&st));
        }
        else if (*prog == '|') {
            push_s64(&st, pop_s64(&st) || pop_s64(&st));
        }
        else if (*prog == '!') {
            push_s64(&st, !pop_s64(&st));
        }
        else if (*prog == '^') {
            s64 a = pop_s64(&st);
            s64 b = pop_s64(&st);
            push_s64(&st, a);
            push_s64(&st, b);
        }
        else if (*prog == ';') {
            s64 a = pop_s64(&st);
            s64 b = pop_s64(&st);
            s64 c = pop_s64(&st);
            push_s64(&st, b);
            push_s64(&st, a);
            push_s64(&st, c);
        }
        else if (*prog == '$') {
            s64 a = pop_s64(&st);
            push_s64(&st, a);
            push_s64(&st, a);
        }
    }
}

int main(int argc, char const *argv[]) {
    if (argc >= 2) {
        int stsz = 4096;
        if (argc >= 3) {
            sscanf(*(argv+2), "%d", &stsz);
        }
        init_stack(&st, 8*stsz);
        FILE *fp = fopen(*(argv+1), "r");
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
    init_stack(&st, 4096);
    while (1) {
        printf("\nkwhy> ");
        char *q = malloc(1024); fgets(q, 1024, stdin);
        interpret(q);
    }
}
