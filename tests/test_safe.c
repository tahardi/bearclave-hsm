#include "../src/safe.h" // Include the internal header

#include <setjmp.h> // NOLINT
#include <stdarg.h> // NOLINT
#include <stddef.h> // NOLINT
#include <stdlib.h>
#include <string.h>

#include <cmocka.h>

static void test_safe_memcpy_happy_path(void **state) {
	// given
	(void)state;

	char dst[10] = {0};
	const char src[] = "hello";

	// when
	int ret = safe_memcpy(dst, sizeof(dst), src, strlen(src));

	// then
	assert_int_equal(ret, SAFE_OK);
	assert_memory_equal(dst, "hello", 5);
}

static void test_safe_memcpy_error_null_arg(void **state) {
	// given
	(void)state;

	char dst[3];
	size_t dst_len = sizeof(dst);

	const char src[] = "hello";
	size_t src_len = strlen(src);

	// when
	int ret = safe_memcpy(dst, dst_len, NULL, src_len);

	// then
	assert_int_equal(ret, SAFE_ERR_NULL_ARGS);
}

static void test_safe_memcpy_error_buffer_overflow(void **state) {
	// given
	(void)state;

	char dst[3];
	size_t dst_len = sizeof(dst);

	const char src[] = "hello";
	size_t src_len = strlen(src);

	// when
	int ret = safe_memcpy(dst, dst_len, src, src_len);

	// then
	assert_int_equal(ret, SAFE_ERR_OVERFLOW);
}

static void test_safe_memset_happy_path(void **state) {
	// given
	(void)state;

	char arr[10];
	size_t arr_len = sizeof(arr);
	size_t count = 5;
	unsigned char val = 'A';

	// when
	int ret = safe_memset(arr, arr_len, count, val);

	// then
	assert_int_equal(ret, SAFE_OK);
	assert_memory_equal(arr, "AAAAA", count);
}

static void test_safe_memset_error_null_arg(void **state) {
	// given
	(void)state;

	char arr[10];
	size_t arr_len = sizeof(arr);
	size_t count = 5;
	unsigned char val = 'A';

	// when
	int ret = safe_memset(NULL, arr_len, count, val);

	// then
	assert_int_equal(ret, SAFE_ERR_NULL_ARGS);
}

static void test_safe_memset_error_buffer_overflow(void **state) {
	// given
	(void)state;

	char arr[10];
	size_t arr_len = sizeof(arr);
	size_t count = arr_len + 1;
	unsigned char val = 'A';

	// when
	int ret = safe_memset(arr, arr_len, count, val);

	// then
	assert_int_equal(ret, SAFE_ERR_OVERFLOW);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_safe_memcpy_happy_path),
		cmocka_unit_test(test_safe_memcpy_error_null_arg),
		cmocka_unit_test(test_safe_memcpy_error_buffer_overflow),
		cmocka_unit_test(test_safe_memset_happy_path),
		cmocka_unit_test(test_safe_memset_error_null_arg),
		cmocka_unit_test(test_safe_memset_error_buffer_overflow),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
