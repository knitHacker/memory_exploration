#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

/***********************
 Code from http://arjunsreedharan.org/post/148675821737/write-a-simple-memory-allocator
************************/


struct header_t {
    size_t size;
    unsigned is_free;
    struct header_t *next;
};


struct header_t *head, *tail;

pthread_mutex_t global_malloc_lock;

struct header_t *get_free_block(size_t size)
{
    struct header_t *curr = head;

    while (curr)
    {
        if (curr->is_free && curr->size >= size)
        {
            return curr;
        }

        curr = curr->next;
    }

    return NULL;
}

// Allocates the requested amount of memory from the heap
// If memory is exhausted returns NULL
void *my_malloc(size_t size)
{
    size_t total_size;
    void *block;
    struct header_t *header;

    // If the size of memory is 0 immediately return NULL
    if (!size)
    {
        return NULL;
    }

    // Lock access to memory for thread safety
    pthread_mutex_lock(&global_malloc_lock);

    header = get_free_block(size);
    // If can reuse a block use that
    if (header)
    {
        // Take the block and mark it as not free
        header->is_free = 0;
        // Must unlock the mutex or can't malloc any more memory
        pthread_mutex_unlock(&global_malloc_lock);
        // Return the memory after the header
        return (void*)(header +1);
    }

    // If we can't use a pre allocated block that was freed
    // Must create a new block with it's own header
    total_size = sizeof(struct header_t) + size;

    // Receive memory from the OS by increasing the virtual memory head by
    // the needed size
    block = sbrk(total_size);

    // If the OS memory has been exhausted return NULL
    if (block == (void*) -1)
    {
        // Don't forget to unlock the mutex
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }

    // Create the header for the new memory
    header = block;
    header->size = size;
    header->is_free = 0;
    header->next = NULL;

    // If head is null (this was the first memory allocated)
    // set the head to the just allocated block
    if (!head)
    {
        head = header;
    }

    // If tail exists (every time but the first memory allocated)
    // set it's next to the just allocated block to continue linked lists
    if (tail)
    {
        tail->next = header;
    }

    // Set tail to the just allocated block
    tail = header;
    // Must unlock mutex
    pthread_mutex_unlock(&global_malloc_lock);
    return (void*)(header + 1);
}

void my_free(void *block)
{
    struct header_t *header, *tmp;
    void *programbreak;

    if (!block)
    {
        return;
    }

    pthread_mutex_lock(&global_malloc_lock);
    header = (struct header_t*)block - 1;

    programbreak = sbrk(0);

    // If block is at the end of the allocated memory give it back to the OS
    if ((char*)block + header->size == programbreak)
    {
        if (head == tail)
        {
            head = tail = NULL;
        } else {
            tmp = head;
            while(tmp)
            {
                if (tmp->next == tail) {
                    tmp->next = NULL;
                    tail = tmp;
                }
                tmp = tmp->next;
            }
        }

        // move the end of the heap back the block size
        sbrk(0 - sizeof(struct header_t) - header->size);
        pthread_mutex_unlock(&global_malloc_lock);
        return;
    }

    // Otherwise set the block to free
    header->is_free = 1;
    pthread_mutex_unlock(&global_malloc_lock);
}


void *my_calloc(size_t num, size_t nsize)
{
    size_t size;
    void *block;
    if (!num || !nsize)
    {
        return NULL;
    }

    size = num * nsize;

    // check for mul overflow
    if (nsize != size / num)
    {
        return NULL;
    }

    block = my_malloc(size);
    if (!block)
    {
        return NULL;
    }

    // Zeroing out the memory
    memset(block, 0, size);
    return block;
}


void *realloc(void *block, size_t size)
{
    struct header_t *header;
    void *ret;
    if (!block || !size)
    {
        return my_malloc(size);
    }

    header = (struct header_t*)block - 1;
    if (header->size >= size)
    {
        return block;
    }

    ret = my_malloc(size);
    if (ret)
    {
        memcpy(ret, block, header->size);
        my_free(block);
    }
    return ret;
}


int main(int argc, char *argv[])
{

    int *array = my_calloc(10, sizeof(int));
    if (array == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        return (-1);
    }

    for(int i; i < 10; i++)
    {
        array[i] = i;
        printf("%d is at %p\n", array[i], &array[i]);
    }

    my_free(array);

    return 0;
}
