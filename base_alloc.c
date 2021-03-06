#include "base_alloc.h"

memory_region_t get_invalid_region() {
	memory_region_t invalid = { MAP_FAILED, 0};
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
			MAP_ANONYMOUS, 0, 0);
	return ret;
}

bool deallocate_region(const memory_region_t *mr) {
	return munmap(mr->base, mr->len) != -1;
}

int main(){
    memory_region_t mem = allocate_region(32);
    return deallocate_region(&mem);
}
