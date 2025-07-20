#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stacks.h"
typedef long long s64;

int dump = 0;

DECL_WHY_STACK_POP(s64)
DECL_WHY_STACK_POP(double)

DECL_WHY_STACK_PUSH(s64)
DECL_WHY_STACK_PUSH(double)

DECL_WHY_STACK_GET(s64)
DECL_WHY_STACK_GET(double)

long length; // length of the file
struct stack st;
char *subr[4096];
int nsr = 0;
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
int interpret(char *prog, FILE *ahmet23) {
    int flag_float = 0, flag_break = 0;
    char *p = prog;
    if (dump) fprintf(ahmet23, "/-----\\\n");
    for (; *prog; ++prog) {
        char *fmt = flag_float ? "%f" : "%lld";
        if (flag_float) {
            if (*prog == '"') { // make string
                char *rq = match(prog, '"', '"');
                push(&st, 0);
                for (char *i = rq-1; i > prog; --i) {
                    if (*i != '\\') push(&st, *i);
                    else {
                        if (*(st.st_en-1) == 'n') *(st.st_en-1) = '\n';
                        if (*(st.st_en-1) == 't') *(st.st_en-1) = '\t';
                        if (*(st.st_en-1) == '\\') *(st.st_en-1) = '\\';
                    }
                }
                prog = rq;
            }
            else if (*prog == 'P') {
                char q;
                while ((q = pop(&st))) putc(q, stdout);
            }
            else if (*prog == 'p') {
                s64 recv = pop_double(&st);
                printf(fmt, recv);
            }
            else if (*prog == 'F') {
                flag_float = 0;
            }
            else if (*prog == 'g') {
                double x; scanf(fmt, &x);
                push_double(&st, x);
            }
            else if (*prog == '#') {
                char *rh = match(prog, '#', '#');
                s64 x = 0;
                s64 s = 1;
                for (char *i = prog+1; i < rh; ++i) {
                    if (*i == '-') s = -1;
                    else x = 10*x + *i - '0';
                }
                x *= s;
                push_s64(&st, x);
                prog = rh;
            }
            else if (*prog == '+') {
                push_double(&st, pop_double(&st) + pop_double(&st));
            }
            else if (*prog == '-') {
                push_double(&st, pop_double(&st) - pop_double(&st));
            }
            else if (*prog == '*') {
                push_double(&st, pop_double(&st) * pop_double(&st));
            }
            else if (*prog == '/') {
                push_double(&st, pop_double(&st) / pop_double(&st));
            }
            else if (*prog == '%') {
                push_s64(&st, pop_s64(&st) % pop_s64(&st));
            }
            else if (*prog == '[') {
                char *rb = match(prog, '[', ']');
                char *q = substr(prog+1, rb-1);
                while (pop_s64(&st)) interpret(q, ahmet23);
                prog = rb;
            }
            else if (*prog == '(') {
                char *rb = match(prog, '(', ')');
                char *q = substr(prog+1, rb-1);
                s64 m = pop_s64(&st);
                for (s64 i = 0; i < m; ++i)
                interpret(q, ahmet23);
                prog = rb;
            }
            else if (*prog == '<') {
                push_s64(&st, pop_double(&st) < pop_double(&st));
            }
            else if (*prog == '>') {
                push_s64(&st, pop_double(&st) > pop_double(&st));
            }
            else if (*prog == '=') {
                push_s64(&st, pop_double(&st) == pop_double(&st));
            }
            else if (*prog == '&') {
                push_s64(&st, pop_s64(&st) && pop_s64(&st));
            }
            else if (*prog == '|') {
                push_s64(&st, pop_s64(&st) || pop_s64(&st));
            }
            else if (*prog == '\'') {
                push(&st, (char)pop_s64(&st));
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
            else if (*prog == '~') {
                s64 shuf_amt = pop_s64(&st);
                s64 q = *get_at_s64(&st, shuf_amt);
                for (int i = shuf_amt; i >= 2; --i) {
                    *get_at_s64(&st, i) = *get_at_s64(&st, i-1);
                }
                *get_at_s64(&st, 1) = q;
            }
            else if (*prog == ':') {
                char *rc = match(prog, ':', ':');
                double s = 1;
                double e = 1;
                double x = 0;
                int postdot = 0;
                for (char *i = prog+1; i < rc; ++i) {
                    if (*i == '-') {
                        s *= -1;
                    } else if (*i == '.') {
                        postdot = 1; 
                    } else if (postdot) {
                        e /= 10.0;
                        x += (double)(*i-'0')*e;
                    } else {
                        x = 10.0*x + (double)(*i-'0');
                    }; printf("%f ", x);
                }
                push_double(&st, s*x);
                prog = rc;
            }
            else if (*prog == '$') {
                push_s64(&st, *(long long *)(st.st_en-8));
            }
            else if (*prog == '?') {
                push_s64(&st, st.st_en-st.st);
            }
            else if (*prog == 'S') {
                char *rs = match(prog, 'S', 's');
                subr[nsr] = substr(prog+1, rs-1);
                nsr++;
            }
            else if (*prog == 'Q') {
                interpret(subr[pop_s64(&st)], ahmet23);
            }
            else if (*prog == 'q') {
                pop_s64(&st);
            }
            else if (*prog == 'b') {
                flag_break = !pop_s64(&st);
            };
        } else {
            if (*prog == '"') { // make string
                char *rq = match(prog, '"', '"');
                push(&st, 0);
                for (char *i = rq-1; i > prog; --i) {
                    if (*i != '\\') push(&st, *i);
                    else {
                        if (*(st.st_en-1) == 'n') *(st.st_en-1) = '\n';
                        if (*(st.st_en-1) == 't') *(st.st_en-1) = '\t';
                        if (*(st.st_en-1) == '\\') *(st.st_en-1) = '\\';
                    }
                }
                prog = rq;
            }
            else if (*prog == 'P') {
                char q;
                while ((q = pop(&st))) putc(q, stdout);
            }
            else if (*prog == 'p') {
                printf(fmt, pop_s64(&st));
            }
            else if (*prog == 'F') {
                flag_float = 1;
            }
            else if (*prog == 'g') {
                s64 x = 0; scanf(fmt, &x);
                push_s64(&st, x);
            }
            else if (*prog == '#') {
                char *rh = match(prog, '#', '#');
                s64 x = 0;
                s64 s = 1;
                for (char *i = prog+1; i < rh; ++i) {
                    if (*i == '-') s = -1;
                    else x = 10*x + *i - '0';
                }
                x *= s;
                push_s64(&st, x);
                prog = rh;
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
                while (pop_s64(&st)) if (interpret(q, ahmet23) == 2) break;
                prog = rb;
            }
            else if (*prog == '(') {
                char *rb = match(prog, '(', ')');
                char *q = substr(prog+1, rb-1);
                s64 m = pop_s64(&st);
                for (s64 i = 0; i < m; ++i) if (interpret(q, ahmet23) == 2) break;
                prog = rb;
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
            else if (*prog == '\'') {
                push(&st, (char)pop_s64(&st));
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
            else if (*prog == '~') {
                s64 shuf_amt = pop_s64(&st);
                s64 q = *get_at_s64(&st, shuf_amt);
                for (int i = shuf_amt; i >= 2; --i) {
                    *get_at_s64(&st, i) = *get_at_s64(&st, i-1);
                }
                *get_at_s64(&st, 1) = q;
            }
            else if (*prog == ':') {
                char *rc = match(prog, ':', ':');
                double x = 0;
                double s = 1;
                double e = 1;
                int postdot = 0;
                for (char *i = prog+1; i < rc; ++i) {
                    if (*i == '-') {
                        s *= -1;
                    } else if (*i == '.') {
                        postdot = 1; 
                    } else if (postdot) {
                        e /= 10.0;
                        x += (double)(*i-'0')*e;
                    } else {
                        x = 10.0*x + (double)(*i-'0');
                    }
                }
                push_double(&st, s*x);
                prog = rc;
            }
            else if (*prog == '$') {
                push_s64(&st, *(long long *)(st.st_en-8));
            }
            else if (*prog == '?') {
                push_s64(&st, st.st_en-st.st);
            }
            else if (*prog == 'S') {
                char *rs = match(prog, 'S', 's');
                subr[nsr] = substr(prog+1, rs-1);
                nsr++;
                prog = rs;
            }
            else if (*prog == 'Q') {
                interpret(subr[pop_s64(&st)], ahmet23);
            }
            else if (*prog == 'q') {
                pop_s64(&st);
            }
            else if (*prog == 'b') {
                flag_break = !pop_s64(&st);
            };
        }
        if (dump) {
            fprintf(ahmet23, "%td %c | ", prog-p, *prog);
            for (s64 *i = (s64 *)st.st; i < (s64 *)st.st_en; i++) { fprintf(ahmet23, "%lld ", *i); }
            fprintf(ahmet23, "\n");
        }
        if (flag_break) { if (dump) fprintf(ahmet23, "\\-----/\n"); return 2; }
    }
    if (dump) fprintf(ahmet23, "\\-----/\n");
    return 0;
}

int main(int argc, char const **argv) {
    FILE *ahmet23 = fopen("dump.txt", "w");
    char *fname = malloc(1024);
    memset(fname, 0, 1024);
    int stsz = 4096;
    int paramno = 0;
    int paramtyp = -1;
    for (int i = 1; i < argc; i++) {
        if (**(argv+i) == '-') {
            switch (*(*(argv+i)+1)) {
            case 'f':
                paramtyp = 0;
                break;

            case 's':
                paramtyp = 1;
                break;

            case 'd':
                dump = 1;
                break;
            
            case 'V':
                printf("I didn't set a version yet lol\n");
                return 0;
            
            case '?':
                printf("whylang interpreter\n-f: set file\n-s: set stack size\n-d: set dump\n-v: set version\n-?: help\n");
                return 0;

            default:
                break;
            }
        } else if (paramtyp == -1) {
            switch (paramno) {
            case 0:
                strcpy(fname, *(argv+i));
                break;
            
            case 1:
                sscanf(*(argv+i), "%d", &stsz);
                break;
            
            default:
                break;
            }
            paramno++;
        } else {
            switch (paramtyp) {
            case 0:
                strncpy(fname, *(argv+i), 1023);
                break;
            
            case 1:
                sscanf(*(argv+i), "%d", &stsz);
                break;
            
            default:
                break;
            }
        }
    }
    init_stack(&st, 8*stsz);
    if (strlen(fname)) {
        FILE *fp = fopen(fname, "w");
        // find how long the file is and dump it all in a char *
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        char *buffer = malloc(length+1);
        *(buffer+length) = 0;
        fread(buffer, 1, length, fp);
        interpret(buffer, ahmet23);
        free(st.st);
        free(buffer);
        free(fname);
        return 0;
    }
    while (1) {
        printf("\nkwhy> ");
        char *q = malloc(1024); fgets(q, 1024, stdin);
        interpret(q, ahmet23);
        free(q);
    }
}
