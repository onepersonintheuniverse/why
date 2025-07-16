#ifndef _WHY_STACKS_H
#define _WHY_STACKS_H
#define DECL_WHY_STACK_POP(T) T pop_##T(struct stack *st) {\
    T val = *(T *)(st->st_en - sizeof(T));\
    for (int i = 0; i < sizeof(T); i++) pop(st);\
    return val;\
}
#define DECL_WHY_STACK_PUSH(T) void push_##T(struct stack *st, T val) {\
    for (int i = 0; i < sizeof(T); i++, val >>= 8) push(st, val&255);\
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
#endif