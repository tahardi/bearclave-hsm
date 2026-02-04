#include "../src/hsm.h"
#include "bearclave/pkcs11.h"

#include <setjmp.h> // NOLINT
#include <stdarg.h> // NOLINT
#include <stddef.h> // NOLINT
#include <stdlib.h>
#include <string.h>

#include <cmocka.h>

static void setup(void) {
	int ret = hsm_initialize();
	assert_int_equal(ret, HSM_OK);
	assert_true(g_hsm.initialized);
}

static void teardown(void) {
	int ret = hsm_finalize();
	assert_int_equal(ret, HSM_OK);
	assert_false(g_hsm.initialized);
}

static void test_hsm_initialize_happy_path(void **state) {
	// given
	(void)state;

	// when
	int ret = hsm_initialize();

	// then
	assert_int_equal(ret, HSM_OK);
	assert_true(g_hsm.initialized);
}

static void test_hsm_finalize_happy_path(void **state) {
	// given
	(void)state;
	setup();

	// when
	int ret = hsm_finalize();

	// then
	assert_int_equal(ret, HSM_OK);
	assert_false(g_hsm.initialized);
}

static void test_hsm_get_info_happy_path(void **state) {
	// given
	(void)state;
	setup();

	// when
	hsm_info_t hsm_info;
	int ret = hsm_get_info(&hsm_info);

	// then
	assert_int_equal(ret, HSM_OK);
	assert_int_equal(hsm_info.ck_version.major, CK_VERSION_MAJOR);
	assert_int_equal(hsm_info.ck_version.minor, CK_VERSION_MINOR);
	assert_int_equal(hsm_info.lib_version.major, LIB_VERSION_MAJOR);
	assert_int_equal(hsm_info.lib_version.minor, LIB_VERSION_MINOR);
	assert_int_equal(hsm_info.flags, FLAGS);
	assert_memory_equal(hsm_info.lib_desc, LIB_DESC, sizeof(LIB_DESC));
	assert_memory_equal(hsm_info.man_id, MAN_ID, sizeof(MAN_ID));
	teardown();
}

static void test_hsm_get_info_error_not_initialized(void **state) {
	// given
	(void)state;
	assert_false(g_hsm.initialized);

	// when
	hsm_info_t hsm_info;
	int ret = hsm_get_info(&hsm_info);

	// then
	assert_int_equal(ret, HSM_ERR_NOT_INITIALIZED);
}

static void test_hsm_get_info_error_bad_args(void **state) {
	// given
	(void)state;
	setup();

	// when
	int ret = hsm_get_info(NULL);

	// then
	assert_int_equal(ret, HSM_ERR_BAD_ARGS);
	teardown();
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_hsm_initialize_happy_path),
		cmocka_unit_test(test_hsm_finalize_happy_path),
		cmocka_unit_test(test_hsm_get_info_happy_path),
		cmocka_unit_test(test_hsm_get_info_error_not_initialized),
		cmocka_unit_test(test_hsm_get_info_error_bad_args),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
