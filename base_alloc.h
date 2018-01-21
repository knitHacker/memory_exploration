

#ifndef BASE_ALLOC_HEADER

#define BASE_ALLOC_HEADER
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>

#include <stdbool.h>

typedef struct memory_region {
    void *base;
    size_t len;
} memory_region_t;

memory_region_t get_invalid_region();

bool is_valid_memory_region(const memory_region_t* mr);

memory_region_t allocate_region(size_t len);

bool deallocate_region(const memory_region_t *mr);

#endif
