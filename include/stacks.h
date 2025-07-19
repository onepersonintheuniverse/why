#ifndef __WHYLANG_STACKS_H
#define __WHYLANG_STACKS_H
#include <stdint.h>
#define _int(x) int##x##_t
#define _uint(x) uint##x##_t
#define _bitcast(T, x) (*(T *)(&x))
#define _bitcast_ptr(T, p) (*(T *)(p))
typedef long long s64;
const int __WHYLANG_CONST_ORZIUM = 23; // easter egg because why not

// since these are multiline C macros, you have to add \ at the end of each line. very stupid.

/**
 * @brief declare a `pop_T` function which pops a value of type `T` from `&st`.
 *
 * @brief e.g. `DECL_WHY_STACK_POP(int)` declares a function by the name & signature
 * @brief `int pop_int(struct stack *st)`.
 *
 *
 * @param st pointer to stack.
 */
#define DECL_WHY_STACK_POP(T) T pop_##T(struct stack *st) {\
    st->st_en -= sizeof(T);\
    T *val = (T *)(st->st_en);\
    return *val;\
}

/**
 * @brief same thing as `DECL_WHY_STACK_POP(T)` except it declares a function to push a type of T.
 *
 * @brief e.g. when int is passed for T, it makes a function `int push_int(struct stack *st, int val)`.
 */
#define DECL_WHY_STACK_PUSH(T) void push_##T(struct stack *st, T val) {\
    s64 q = _bitcast(s64, val);\
    for (int i = 0; i < sizeof(T); i++, q >>= 8) push(st, q&255);\
}

/**
 * @brief same thing as `DECL_WHY_STACK_POP(T)` except it declares a function to retrieve xth top value of type T.
 *
 * @brief e.g. when int is passed for T, it makes a function `int *get_at_int(struct stack *st, long long x)`.
 */
#define DECL_WHY_STACK_GET(T) T *get_at_##T(struct stack *st, long long x) {\
    return (T *)(st->st_en-x*sizeof(T));\
}

struct stack {
    char *st;
    char *st_en;
    size_t sz;
};

void init_stack(struct stack *st, size_t sz) {
    st->st_en = st->st = malloc(sz);
    st->sz = sz;
}

void push(struct stack *st, char x) { // push to the stack
    if (st->st_en == st->st + st->sz) return;
    *(st->st_en) = x;
    ++st->st_en;
}

char pop(struct stack *st) { // pop from the stack and return the popped element
    if (st->st_en == st->st) return 0;
    --st->st_en;
    return *st->st_en;
}

char *get_at(struct stack *st, int x) { // get xth char on stack from top
    return st->st_en-x;
}
#endif
