#include "interface.h"
#include "../types/error.h"

int fd_write(FileDescriptor fd, uint32 count, void* buffer) {
    if (SERVICE_WRITE >= fd.count) {
        return E_UNKNOWN_SERVICE;
    }

    Operation operation = fd.operations[SERVICE_WRITE];
    struct ServiceWriteData data = {fd.state, count, buffer};
    return operation(&data);
}
