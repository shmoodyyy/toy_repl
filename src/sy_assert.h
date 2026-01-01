#ifndef ASSERTS_H
#define ASSERTS_H

#include <stdlib.h>
#include <stdio.h>

void abort_impl_bug(const char* msg)
{
    printf("Implementation Error abort(): %s\n", msg);
    abort();
}

#endif
