#include "dump.h"
#include "../std/format.h"

void print_hex(FileDescriptor fd, const char* title, void* buf, int count) {
    fd_format(fd, "Dump of %s:", title);
    for (int r = 0; r < count / 0x10; r++) {
        char* p = buf + r * 0x10;
        fd_format(
            fd,
            "%2x: %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x",
            r * 0x10,
            p[0], p[1], p[2], p[3],
            p[4], p[5], p[6], p[7],
            p[8], p[9], p[10], p[11],
            p[12], p[13], p[14], p[15]
        );
    }
}
