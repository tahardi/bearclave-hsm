#include "../src/hsm.h" // Include the internal header
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

// test_hsm_happy_paths The test code for the HSM info getters is very basic and
// repetitive. Thus, all happy path and error tests have been grouped together
// to reduce code re-use.
static void test_hsm_happy_paths(void **state) {
	// given
	(void)state;
	setup();

	// when/then
	unsigned char major = 0;
	int ret = hsm_get_ck_version_major(&major);
	assert_int_equal(ret, HSM_OK);
	assert_int_equal(major, g_ck_version_major);

	unsigned char minor = 0;
	ret = hsm_get_ck_version_minor(&minor);
	assert_int_equal(ret, HSM_OK);
	assert_int_equal(minor, g_ck_version_minor);

	unsigned long flags = 0;
	ret = hsm_get_flags(&flags);
	assert_int_equal(ret, HSM_OK);
	assert_int_equal(flags, g_flags);

	unsigned char lib_desc[LIBRARY_DESCRIPTION_SIZE];
	ret = hsm_get_lib_desc(lib_desc, sizeof(lib_desc));
	assert_int_equal(ret, HSM_OK);
	assert_memory_equal(lib_desc, g_lib_desc, sizeof(g_lib_desc));

	ret = hsm_get_lib_version_major(&major);
	assert_int_equal(ret, HSM_OK);
	assert_int_equal(major, g_lib_version_major);

	ret = hsm_get_lib_version_minor(&minor);
	assert_int_equal(ret, HSM_OK);
	assert_int_equal(minor, g_lib_version_minor);

	unsigned char man_id[MANUFACTURER_ID_SIZE];
	ret = hsm_get_man_id(man_id, sizeof(man_id));
	assert_int_equal(ret, HSM_OK);
	assert_memory_equal(man_id, g_man_id, sizeof(g_man_id));

	teardown();
}

static void test_hsm_errors_not_initialized(void **state) {
	// given
	(void)state;
	assert_false(g_hsm.initialized);

	// when/then
	int ret = hsm_finalize();
	assert_int_equal(ret, HSM_ERR_NOT_INITIALIZED);

	unsigned char major = 0;
	ret = hsm_get_ck_version_major(&major);
	assert_int_equal(ret, HSM_ERR_NOT_INITIALIZED);

	ret = hsm_get_lib_version_major(&major);
	assert_int_equal(ret, HSM_ERR_NOT_INITIALIZED);

	unsigned char minor = 0;
	ret = hsm_get_ck_version_minor(&minor);
	assert_int_equal(ret, HSM_ERR_NOT_INITIALIZED);

	ret = hsm_get_lib_version_minor(&minor);
	assert_int_equal(ret, HSM_ERR_NOT_INITIALIZED);

	unsigned long flags = 0;
	ret = hsm_get_flags(&flags);
	assert_int_equal(ret, HSM_ERR_NOT_INITIALIZED);

	unsigned char lib_desc[LIBRARY_DESCRIPTION_SIZE];
	ret = hsm_get_lib_desc(lib_desc, sizeof(lib_desc));
	assert_int_equal(ret, HSM_ERR_NOT_INITIALIZED);

	unsigned char man_id[MANUFACTURER_ID_SIZE];
	ret = hsm_get_man_id(man_id, sizeof(man_id));
	assert_int_equal(ret, HSM_ERR_NOT_INITIALIZED);
}

static void test_hsm_errors_bad_args(void **state) {
	// given
	(void)state;
	setup();

	// when/then
	int ret = hsm_get_ck_version_major(NULL);
	assert_int_equal(ret, HSM_ERR_BAD_ARGS);

	ret = hsm_get_lib_version_major(NULL);
	assert_int_equal(ret, HSM_ERR_BAD_ARGS);

	ret = hsm_get_ck_version_minor(NULL);
	assert_int_equal(ret, HSM_ERR_BAD_ARGS);

	ret = hsm_get_lib_version_minor(NULL);
	assert_int_equal(ret, HSM_ERR_BAD_ARGS);

	ret = hsm_get_flags(NULL);
	assert_int_equal(ret, HSM_ERR_BAD_ARGS);

	ret = hsm_get_lib_desc(NULL, 0);
	assert_int_equal(ret, HSM_ERR_BAD_ARGS);

	ret = hsm_get_man_id(NULL, 0);
	assert_int_equal(ret, HSM_ERR_BAD_ARGS);

	teardown();
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_hsm_initialize_happy_path),
		cmocka_unit_test(test_hsm_finalize_happy_path),
		cmocka_unit_test(test_hsm_happy_paths),
		cmocka_unit_test(test_hsm_errors_not_initialized),
		cmocka_unit_test(test_hsm_errors_bad_args),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
