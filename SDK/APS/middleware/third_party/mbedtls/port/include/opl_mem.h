#ifndef _OPL_MBEDTLS_MEM_H_
#define _OPL_MBEDTLS_MEM_H_

#include <stdlib.h>

void *opl_mbedtls_mem_calloc(size_t num, size_t nsize);
void opl_mbedtls_mem_free(void *ptr);

#endif
