#include <stdio.h>
#include <string.h>
#define STACK 1024

char stack[STACK] = {};
int stack_end = 0;
long filesize;

void push(char x) {
    if (stack_end == 1024) return;
    stack[stack_end] = x;
    ++stack_end;
}

void pop() {
    if (!stack_end) return;
    --stack_end;
    stack[stack_end] = 0;
}

void match() {}

void interpret(char *contents) {

}

int main(int argc, char const *argv[]) {
    FILE *fp = fopen("helloworld.why", "r");
    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *buffer = malloc(filesize+1);
    *(buffer+filesize) = 0;
    fread(buffer, 1, filesize, fp);
    interpret(buffer);
    return 0;
}
