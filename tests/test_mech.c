#include "../src/common.h"
#include "../src/hsm.h"

#include <setjmp.h> // NOLINT
#include <stdarg.h> // NOLINT
#include <stddef.h> // NOLINT
#include <stdlib.h>

#include <cmocka.h>
#include <stdio.h>

static void test_mech_get_info_happy_path(void **state) {
	// given
	(void)state;
	unsigned long type = 8;
	mech_t *mech = mech_new(type);
	assert_non_null(mech);

	// when
	mech_info_t mech_info;
	int err = mech_get_info(mech, &mech_info);

	// then
	assert_int_equal(err, MECH_OK);
	assert_int_equal(mech_info.type, type);
	assert_int_equal(mech_info.ulMaxKeySize, MECH_MAX_KEY_SIZE);
	assert_int_equal(mech_info.ulMinKeySize, MECH_MIN_KEY_SIZE);
	mech_free(mech);
}

static void test_mech_get_info_error_bad_args(void **state) {
	// given
	(void)state;

	// when
	int err = mech_get_info(NULL, NULL);

	// then
	assert_int_equal(err, MECH_ERR_BAD_ARGS);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_mech_get_info_happy_path),
		cmocka_unit_test(test_mech_get_info_error_bad_args),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
