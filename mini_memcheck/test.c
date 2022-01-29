/**
 * mini_memcheck
 * CS 241 - Fall 2021
 */
#include <stdio.h>
#include <stdlib.h>

int main() {
    void *p1 = malloc(30);
    //void *p2 = malloc(40);
    void *p3 = malloc(50);
    p3 = realloc(p3, 20);
    //free(p3);
    return 0;
}