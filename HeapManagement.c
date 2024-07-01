#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>

typedef enum{false=0, true=1} bool;

#define max_heap_size 100//here we take 100 bits to check the proper working of the program
//the maximum size of memory for the function

typedef struct mem_info_t 
{
    struct mem_info_t *next;
    //contains a pointer to the next memory block
    size_t size;
    //size of the block
    bool is_free;
    //flag indicating whether the block is free.
}mem_info;
//declared a structure to have the metadata for the memory blocks

//this structure takes 12 bytes

typedef struct heap_tag
{
    struct mem_info_t *root;
    char* heap_end;
}Heap;


char heap_memory[max_heap_size];
//simulated the memory of 10kb for the program
char heap_object[sizeof(Heap)];
//we create an array of the size of the structure heap to get that size from the heap

void clear_heap_memory()
{
    for(size_t i=0; i<max_heap_size; i++)
    {
        heap_memory[i]=0;
    }
}

bool is_init=false;
Heap* heap= NULL;

Heap* get_heap_instance() 
{
    if (is_init == false) 
    {
        heap = (Heap*)malloc(sizeof(Heap));
        // Initialize the heap here if needed
        is_init = true;
    }
    return heap;
}

void heap_init() 
{

    clear_heap_memory();
    Heap* heap=get_heap_instance();
    heap->root = (struct mem_info_t*)&heap_memory[0];
    heap->root->next=NULL;
    heap->root->size=max_heap_size;
    heap->root->is_free=1;

    heap->heap_end=&heap_memory[max_heap_size-1];
    printf("\nHeap Initialized\n");
}

void* alloc_memory(size_t size)
{
    char* mem = NULL;
    mem_info* block_info = get_heap_instance()->root;

    do
    {
        if (block_info->is_free == true && block_info->size >= size)
        {
            // Check if the remaining size is large enough for a new block_info
            size_t rem_size = block_info->size - size;

            if (rem_size < sizeof(mem_info))
            {
                // Not enough space for a new block_info
                printf("Error: Insufficient memory to allocate %zu bytes.\n", size);
                return NULL;
            }

            mem = (char*)(block_info + 1);
            block_info->size = size;
            block_info->is_free = false;

            mem_info* temp_new = (mem_info*)(mem + size);
            temp_new->next = block_info->next;
            temp_new->is_free = true;
            temp_new->size = rem_size - sizeof(mem_info);

            block_info->next = temp_new;

            break;
        }

        block_info = block_info->next;
    } while (block_info != NULL);

    if (mem == NULL) {
        // Allocation failed
        printf("Error: Insufficient memory to allocate %zu bytes.\n", size);
    }

    return (void*)mem;
}

bool dealloc_memory(void* this_mem) {
    char* mem = (char*)this_mem;
    bool status = false;

    if ((size_t)mem < (size_t)heap_memory) {
        printf("\nInvalid memory\n");
        return status;
    }

    mem_info* block_info = get_heap_instance()->root;
    mem_info* required_mem_block = (mem_info*)((size_t)mem - sizeof(mem_info));

    do {
        if (block_info == required_mem_block) {
            printf("\nFound the memory to be deallocated at %p\n\n", (void*)required_mem_block);
            block_info->is_free = true;

            // clear the memory
            for (size_t c = 0; c < block_info->size; ++c) {
                mem[c] = 0;
            }

            status = true;
            break;
        }

        block_info = block_info->next;
    } while (block_info != NULL);


    return status;
}


void heap_dump() {
    printf("\nHeap Information\n");
    size_t size_of_mem_info = sizeof(mem_info);
    printf("\nSize of mem_info(metadata): %zu bytes\n\n", size_of_mem_info);

    mem_info* block_info = get_heap_instance()->root;

    while (block_info != NULL) {
        printf("Block at Address %p:\n", (void*)block_info);
        printf("    Size: %zu\n", block_info->size);
        printf("    Free: %s\n", block_info->is_free ? "true" : "false");
        printf("    Content:");

        for (size_t i = 0; i < block_info->size; i++) {
            printf(" %02x", 0xFF & (unsigned int)(*((char*)block_info + sizeof(mem_info) + i)));
        }

        printf("\n");

        block_info = block_info->next;
    }

    printf("\n");
}



void merge_free_blocks() {
    printf("Merging free blocks...\n");

    mem_info* block_info = get_heap_instance()->root;

    while (block_info != NULL && block_info->next != NULL) {
        mem_info* current_block = block_info;
        mem_info* next_block = (mem_info*)block_info->next;

        if (current_block->is_free && next_block->is_free) {
            // Merge the free blocks
            printf("Merging blocks at addresses %p and %p\n", (void*)current_block, (void*)next_block);
            current_block->size += sizeof(mem_info) + next_block->size;
            current_block->next = next_block->next;
            free(next_block);
        } else {
            // Move to the next pair of blocks
            block_info = block_info->next;
        }
    }

    printf("Merging complete.\n");
}


int main() {
    // Initialize the heap
    printf("\nInitializing the heap\n");
    heap_init();
    printf("\n-------------------------------------------------------------------------------------\n\n");
    
    // Allocate memory
    printf("Allocating memory mem1 of 20 bits\n");
    void* mem1 = alloc_memory(20);
    printf("Allocating memory mem2 of 10 bits\n");
    void* mem2 = alloc_memory(10);
    printf("Allocating memory mem3 of 10 bits\n");
    void* mem3 = alloc_memory(10);
    printf("\n-------------------------------------------------------------------------------------\n\n");

    // Print heap state
    printf("Heap State After Allocation:\n");
    heap_dump();
    printf("\n-------------------------------------------------------------------------------------\n\n");

    // Deallocate memory
    printf("Deallocating memory mem1\n");
    dealloc_memory(mem1);
    printf("Heap State After Deallocation:\n");
    heap_dump();
    printf("\n-------------------------------------------------------------------------------------\n\n");

    // Allocate more memory
    printf("Allocating more memory of 30 bits\n");
    void* mem4 = alloc_memory(30);
    printf("Heap State After Allocation:\n");
    heap_dump();
    printf("\n-------------------------------------------------------------------------------------\n\n");

    printf("Deallocating memory mem2");
    dealloc_memory(mem2);
    printf("Heap State After Deallocation:\n");
    heap_dump();
    printf("\n-------------------------------------------------------------------------------------\n\n");
  
    // Merge free blocks if needed
    printf("To Merge free blocks\n");
    merge_free_blocks();
    printf("\n-------------------------------------------------------------------------------------\n\n");

    // Print final heap state after merging
    printf("Heap State After Merging Free Blocks:\n");
    heap_dump();
    printf("\n-------------------------------------------------------------------------------------\n\n");

    return 0;
}