#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>

typedef struct memory_region {
	void *base;
	size_t len;
} memory_region_t;

void *my_alligned_alloc(const memory_region_t *mr, size_t len, size_t alignment);

void *my_alloc(size_t len);

void my_dealloc(void *mem);

