#include <stdio.h>

#define LOG_ERR(s) fprintf(stderr, "Error: %s\n", s)
#define TODO(s) fprintf(stderr, "TODO at %s:%d => %s\n", __FILE__, __LINE__, s); exit(1);

#define MALLOC(size) ({ \
    void *ptr = malloc(size); \
    if (!ptr) { \
        fprintf(stderr, "Failed to allocate memory: %s:%d\n", __FILE__, __LINE__);\
        exit(EXIT_FAILURE); \
    } \
    ptr; \
})
