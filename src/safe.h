#ifndef BEARCLAVE_SAFE_H
#define BEARCLAVE_SAFE_H

#include <stddef.h>

typedef enum {
	SAFE_OK = 0,
	SAFE_ERR_NULL_ARGS = 1,
	SAFE_ERR_OVERFLOW = 2,
} safe_error_t;

safe_error_t safe_memcpy(void *dst, size_t dst_len, const void *src, size_t src_len);
safe_error_t safe_memset(void *arr, size_t arr_len, size_t count, unsigned char val);

#endif // BEARCLAVE_SAFE_H
