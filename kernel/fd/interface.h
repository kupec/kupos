#pragma once

#include "../types/types.h"

#define SERVICE_WRITE 0

typedef int (*Operation)(void* state);

typedef struct _FileDescriptor {
    void* state;
    Operation* operations;
    uint32 count;
} FileDescriptor;

struct ServiceWriteData {
    uint32 count;
    void* buffer;
};

int fd_write(FileDescriptor fd, uint32 count, void* buffer);
