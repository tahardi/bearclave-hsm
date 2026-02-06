#include "safe.h"

safe_error_t safe_memcpy(void *dst, size_t dst_len, const void *src,
			 size_t src_len) {
	if (dst == NULL || src == NULL) {
		return SAFE_ERR_NULL_ARGS;
	}
	if (dst_len < src_len) {
		return SAFE_ERR_OVERFLOW;
	}

	unsigned char *dst_bytes = (unsigned char *)dst;
	const unsigned char *src_bytes = (const unsigned char *)src;
	for (size_t i = 0; i < src_len; i++) {
		dst_bytes[i] = src_bytes[i];
	}
	return SAFE_OK;
}

safe_error_t safe_memcpy_with_padding(void *dst, size_t dst_len,
				      const void *src, size_t src_len,
				      unsigned char padding) {
	if (dst == NULL || src == NULL) {
		return SAFE_ERR_NULL_ARGS;
	}
	if (dst_len < src_len) {
		return SAFE_ERR_OVERFLOW;
	}

	unsigned char *dst_bytes = (unsigned char *)dst;
	safe_memcpy(dst_bytes, dst_len, src, src_len);
	safe_memset(dst_bytes + src_len, dst_len - src_len, dst_len - src_len,
		    padding);
	return SAFE_OK;
}

// NOLINTBEGIN(bugprone-easily-swappable-parameters)
safe_error_t safe_memset(void *arr, size_t arr_len, size_t count,
			 unsigned char val) {
	if (arr == NULL) {
		return SAFE_ERR_NULL_ARGS;
	}
	if (arr_len < count) {
		return SAFE_ERR_OVERFLOW;
	}

	unsigned char *arr_bytes = (unsigned char *)arr;
	for (size_t i = 0; i < count; i++) {
		arr_bytes[i] = val;
	}
	return SAFE_OK;
}
// NOLINTEND(bugprone-easily-swappable-parameters)
