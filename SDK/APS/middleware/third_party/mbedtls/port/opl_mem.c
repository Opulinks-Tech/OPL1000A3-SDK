#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "opl_mem.h"

void *opl_mbedtls_mem_calloc(size_t num, size_t nsize)
{
    void *buf = NULL;
    size_t size;

    if (num == 0 || nsize == 0) {
        return NULL;
    }

	size = num * nsize;

	buf = malloc(size);
	if (!buf) {
		return NULL;
    }    
    else {
	    memset(buf, 0, size);
	}
	return buf;
}


void opl_mbedtls_mem_free(void *ptr)
{
    free(ptr);
}

