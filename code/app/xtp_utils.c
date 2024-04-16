#include "xtp_utils.h"
#include <string.h>

void swap_num(void* v1, void* v2, size_t size)
{
    char buf[size];
    memcpy(buf, v1, size);
    memcpy(v1, v2, size);
    memcpy(v2, buf, size);
}
