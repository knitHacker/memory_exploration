#include <stdio.h>
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

void *malloc(size_t size)
{
    size_t total_size;
    void *block;
    struct header_t *header;

    if (!size)
    {
        return NULL;
    }

    pthread_mutex_lock(&global_malloc_lock);

    header = get_free_block(size);
    // If can reuse a block use that
    if (header)
    {
        header->is_free = 0;
        pthread_mutex_unlock(&global_malloc_lock);
        return (void*)(header +1);
    }

    total_size = sizeof(struct header_t) + size;

    block = sbrk(total_size);

    if (block == (void*) -1)
    {
        pthread_mutex_unlock(&global_malloc_lock);
        return NULL;
    }

    header = block;
    header->size = size;
    header->is_free = 0;
    header->next = NULL;

    if (!head)
    {
        head = header;
    }

    if (tail)
    {
        tail->next = header;
    }

    tail = header;
    pthread_mutex_unlock(&global_malloc_lock);
    return (void*)(header + 1);
}

int main(int argc, char *argv[])
{

    int *array = malloc(10*sizeof(int));
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

    return 0;
}
