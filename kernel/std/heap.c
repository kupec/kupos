#include "heap.h"
#include "../devices/memory/memory.h"

#define HEAP_STACK_PAGES 10
#define HEAP_PAGES 90
#define HEAP_TOTAL_PAGES (HEAP_STACK_PAGES + HEAP_PAGES)

typedef struct {
    void* ptr;
    int length;
    bool free;
} Chunk;

typedef struct {
    Chunk *list;
    uint32 top;
    uint32 capacity;
} HeapStack;

HeapStack heap_stack;

int push_chunk(Chunk chunk) {
    if (heap_stack.top == heap_stack.capacity) {
        return E_MEMORY_EXCEED;
    }

    heap_stack.list[heap_stack.top++] = chunk;
    return E_OK;
}

int pop_chunk(Chunk *out) {
    if (heap_stack.top == 0) {
        return E_MEMORY_EXCEED;
    }

    *out = heap_stack.list[--heap_stack.top];
    return E_OK;
}

static uint32 stack_size() {
    return heap_stack.top;
}

int heap_init() {
    void* heap;
    int err = memory_alloc_pages(HEAP_TOTAL_PAGES, &heap);
    if (err < 0) {
        return err;
    }

    int stack_bytes_size = HEAP_STACK_PAGES * MEM_PAGE_SIZE;
    heap_stack.list = heap;
    heap_stack.top = 0;
    heap_stack.capacity = stack_bytes_size / sizeof(Chunk);

    Chunk full_chunk = {
        .ptr = heap + stack_bytes_size,
        .length = HEAP_PAGES * MEM_PAGE_SIZE,
        .free = true
    };
    err = push_chunk(full_chunk);
    return err;
}

void* malloc(int size) {
    Chunk *p_chunk = 0;
    for (int i = 0; i < stack_size(); i++) {
        p_chunk = &heap_stack.list[i];
        if (p_chunk->free && p_chunk->length >= size) {
            break;
        }
    }
    if (p_chunk == 0) {
        return 0;
    }

    int free_chunk_length = p_chunk->length;
    p_chunk->length = size;
    p_chunk->free = false;

    if (free_chunk_length > size) {
        Chunk remain_chunk = {
            .ptr = p_chunk->ptr + p_chunk->length,
            .length = free_chunk_length - size,
            .free = true
        };
        if (push_chunk(remain_chunk) < 0) {
            return 0;
        }
    }

    return p_chunk->ptr;
}

void free(void* ptr) {
    for (int i = 0; i < stack_size(); i++) {
        Chunk *p_chunk = &heap_stack.list[i];
        if (p_chunk->ptr == ptr) {
            p_chunk->free = true;
            return;
        }
    }
}
