#pragma once

#include "../fd/interface.h"

void print_hex(FileDescriptor fd, const char* title, void* buf, int count);
