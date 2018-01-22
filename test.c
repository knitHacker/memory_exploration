
#include <stdlib.h>

/****************************************************************************
 Program for testing memory management code:
    
******************************************************************************/

unsigned NUM_OF_TEST_VARIABLES = 10

enum action {STORE, READ, FREE};    

struct memory_t {
    unsigned is_free;
    size_t size;
    int *data;
}

void store(struct memory_t* test)
{
    for (int i=0; i < test->size; i++)
    {
        test->data[i] = i;
    }
}

void read(struct memory_t* test)
{
    for (int i=0; i < test->size; i++)
    {
        printf("Reading value %d from %p.\n", test->data[i], test.data);
    }
}

void alloc(struct memory_t* test)
{
    size_t size = rand();
    test->data = my_alloc(size*sizeof(int));
    test->size = size;
}

void free_test(struct memory_t* test)
{
    my_free(test->data);
    test->is_free = 1;
    test->size = 0;
}

int main(int argc, char *argv[])
{

    // seeding the random number generator 
    // TODO: Use an argument for the seed
    srand(42);
    
    struct memory_t tests[NUM_OF_TEST_VARIABLES];

    for (int i=0; i < NUM_OF_TEST_VARIABLES; i++)
    {
        tests[i].is_free = 1;
        tests[i].size = 0;
        tests[i].data = NULL;
    }

    int i = 0;
    int r;
    while (i < 1000)
    {
        r = rand();
        if (tests[r].is_free)
        {
            alloc(&tests[r]);
        } else
        {
            int t = rand();
            switch(t%3):
                STORE: store(&tests[r]); break;
                READ: read(&tests[r]); break;
                FREE: free_test(&tests[r]); break;
        }
        i++;
    }
    
    for (int i=0; i < NUM_OF_TEST_VARIABLES; i++)
    {
        if (!tests[i]->is_free)
        {
            free(tests[i]->data);
        }
    }

    return 0;
}
