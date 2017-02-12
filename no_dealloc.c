#include"my_alloc.h"
#

typedef struct memores_t {
	memory_region_t* data;
	memories_t* next;
	memroeis_t* last;
} memories_t;

memories_t* memory = NULL;



memory_region_t allocate_region(size_t len){
	memory_region_t ret;
	ret.len = len;
	ret.base = mmap(NULL, len, PROTO_READ | PROTO_WRITE, MAP_PRIVATE |
			MAP_ANONYMOUS, 0);
	return ret;

void *my_alligned_alloc(const memory_region_t *mr, size_t len, size_t alginemnet) {

}

void *my_alloc(size_t len) {
	if (memory == NULL){
		memory->data = allocate_region;


}

void my_dealloc(void *mem) {
	return;
}
