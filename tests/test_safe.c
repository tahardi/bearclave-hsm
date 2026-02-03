#include "../src/safe.h"  // Include the internal header

#include <setjmp.h> // NOLINT
#include <stdarg.h> // NOLINT
#include <stddef.h> // NOLINT
#include <stdlib.h>
#include <string.h>

#include <cmocka.h>

static void test_safe_memcpy_success(void **state) {
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

static void test_safe_memcpy_null_arg(void **state) {
	// given
	(void)state;

	char dst[3];
	const char src[] = "hello";

	// when
	int ret = safe_memcpy(dst, sizeof(dst), 0, strlen(src));

	// then
	assert_int_equal(ret, SAFE_ERR_NULL_ARGS);
}

static void test_safe_memcpy_buffer_overflow(void **state) {
	// given
	(void)state;

	char dst[3];
	const char src[] = "hello";

	// when
	int ret = safe_memcpy(dst, sizeof(dst), src, strlen(src));

	// then
	assert_int_equal(ret, SAFE_ERR_OVERFLOW);
}

static void test_safe_memset_success(void **state) {
	// given
	(void)state;
	char arr[10];

	// when
	int ret = safe_memset(arr, sizeof(arr), 'A', 5);

	// then
	assert_int_equal(ret, SAFE_OK);
	assert_memory_equal(arr, "AAAAA", 5);
}

static void test_safe_memset_null_arg(void **state) {
	// given
	(void)state;
	char arr[10];

	// when
	int ret = safe_memset(0, sizeof(arr), 'A', 5);

	// then
	assert_int_equal(ret, SAFE_ERR_NULL_ARGS);
}

static void test_safe_memset_buffer_overflow(void **state) {
	// given
	(void)state;
	char arr[10];

	// when
	int ret = safe_memset(arr, sizeof(arr), 'A', sizeof(arr) + 1);

	// then
	assert_int_equal(ret, SAFE_ERR_OVERFLOW);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_safe_memcpy_success),
		cmocka_unit_test(test_safe_memcpy_null_arg),
		cmocka_unit_test(test_safe_memcpy_buffer_overflow),
		cmocka_unit_test(test_safe_memset_success),
		cmocka_unit_test(test_safe_memset_null_arg),
		cmocka_unit_test(test_safe_memset_buffer_overflow),
	    };
	return cmocka_run_group_tests(tests, NULL, NULL);
}
