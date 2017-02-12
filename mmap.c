#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>

typedef struct memory_region {
	void *base;
	size_t len;
	size_t allocated;
	struct memory_region *next;
} memory_region_t;

memory_region_t get_invalid_region() {
	memory_region_t invalid = { MAP_FAILED, 0 };
	return invalid;
}

bool is_valid_memory_region(const memory_region_t* mr) {
	memory_region_t invalid = get_invalid_region();
	return mr->base == invalid.base && mr->len == invalid.len;
}

memory_region_t allocate_region(size_t len) {
	memory_region_t ret;
	ret.len = len;

	ret.base = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_PRIVATE |
			MAP_ANONYMOUS, 0);
	return ret;
}

bool deallocate_region(const memory_region_t *mr) {
	return munmap(mr->base, mr->len) != -1;
}


void *aligned_alloc(const memory_region_t *mr, size_t len, size_t alignment) {
	// TODO: Make len a multiple of alignment.
	// TODO: If there is enough memory in the region, allocate from there, and
	// increase allocated inside mr.
	// TODO: Else, allocate a new region, point mr->next to that, then recurse.
}
