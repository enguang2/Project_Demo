/**
 * nonstop_networking
 * CS 241 - Fall 2021
 */
#pragma once
#include <stddef.h>
#include <sys/types.h>

#define LOG(...)                      \
    do {                              \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n");        \
    } while (0);

typedef enum { GET, PUT, DELETE, LIST, V_UNKNOWN } verb;
//wrappers
ssize_t write_to_socket(int , const char *, size_t );
ssize_t read_from_socket(int , char *, size_t );
size_t read_head(int socket, char *buffer, size_t count);
int error_dectect(size_t size1, size_t size2);/**
 * nonstop_networking
 * CS 241 - Fall 2021
 */
#pragma once
#include <stddef.h>
#include <sys/types.h>

#define LOG(...)                      \
    do {                              \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n");        \
    } while (0);

typedef enum { GET, PUT, DELETE, LIST, V_UNKNOWN } verb;
//wrappers
ssize_t write_to_socket(int , const char *, size_t );
ssize_t read_from_socket(int , char *, size_t );
size_t read_head(int socket, char *buffer, size_t count);
int error_dectect(size_t size1, size_t size2);