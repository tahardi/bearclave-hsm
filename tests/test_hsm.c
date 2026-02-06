#include "../src/common.h"
#include "../src/hsm.h"
#include "../src/safe.h"
#include "bearclave/pkcs11.h"

#include <setjmp.h> // NOLINT
#include <stdarg.h> // NOLINT
#include <stddef.h> // NOLINT
#include <stdlib.h>
#include <string.h>

#include <cmocka.h>
#include <stdio.h>

static void setup(void) {
	int ret = hsm_initialize();
	assert_int_equal(ret, HSM_OK);
	assert_true(hsm_is_initialized());
}

static void teardown(void) {
	int ret = hsm_finalize();
	assert_int_equal(ret, HSM_OK);
	assert_false(hsm_is_initialized());
}

static void test_hsm_initialize_happy_path(void **state) {
	// given
	(void)state;

	// when
	int ret = hsm_initialize();

	// then
	assert_int_equal(ret, HSM_OK);
	assert_true(hsm_is_initialized());
}

static void test_hsm_finalize_happy_path(void **state) {
	// given
	(void)state;
	setup();

	// when
	int ret = hsm_finalize();

	// then
	assert_int_equal(ret, HSM_OK);
	assert_false(hsm_is_initialized());
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
	assert_false(hsm_is_initialized());

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

static void test_hsm_add_slot_happy_path(void **state) {
	// given
	(void)state;
	setup();

	unsigned long slot_id = 8;
	unsigned char slot_desc[SLOT_DESC_SIZE];
	int err = safe_memcpy_with_padding(slot_desc, SLOT_DESC_SIZE,
					   "slot description",
					   strlen("slot description"), PAD_VAL);
	assert_int_equal(err, SAFE_OK);

	hsm_info_t hsm_info;
	err = hsm_get_info(&hsm_info);
	assert_int_equal(err, HSM_OK);

	slot_t *slot = slot_new(slot_id, slot_desc, hsm_info.man_id);
	assert_non_null(slot);

	unsigned long slots_len = 0;
	err = hsm_get_slots_len(false, &slots_len);
	assert_int_equal(err, HSM_OK);
	assert_int_equal(slots_len, 0);

	// when
	err = hsm_add_slot(slot);

	// then
	assert_int_equal(err, HSM_OK);

	err = hsm_get_slots_len(false, &slots_len);
	assert_int_equal(err, HSM_OK);
	assert_int_equal(slots_len, 1);
}

static void test_hsm_add_slot_error_already_exists(void **state) {
	// given
	(void)state;
	setup();

	unsigned long slot_id = 8;
	unsigned char slot_desc[SLOT_DESC_SIZE];
	int err = safe_memcpy_with_padding(slot_desc, SLOT_DESC_SIZE,
					   "slot description",
					   strlen("slot description"), PAD_VAL);
	assert_int_equal(err, SAFE_OK);

	hsm_info_t hsm_info;
	err = hsm_get_info(&hsm_info);
	assert_int_equal(err, HSM_OK);

	slot_t *slot = slot_new(slot_id, slot_desc, hsm_info.man_id);
	assert_non_null(slot);

	unsigned long slots_len = 0;
	err = hsm_get_slots_len(false, &slots_len);
	assert_int_equal(err, HSM_OK);
	assert_int_equal(slots_len, 0);

	// when
	err = hsm_add_slot(slot);
	assert_int_equal(err, HSM_OK);

	err = hsm_get_slots_len(false, &slots_len);
	assert_int_equal(err, HSM_OK);
	assert_int_equal(slots_len, 1);

	err = hsm_add_slot(slot);
	// then
	assert_int_equal(err, HSM_ERR_SLOT_ALREADY_EXISTS);

	err = hsm_get_slots_len(false, &slots_len);
	assert_int_equal(err, HSM_OK);
	assert_int_equal(slots_len, 1);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_hsm_initialize_happy_path),
		cmocka_unit_test(test_hsm_finalize_happy_path),
		cmocka_unit_test(test_hsm_get_info_happy_path),
		cmocka_unit_test(test_hsm_get_info_error_not_initialized),
		cmocka_unit_test(test_hsm_get_info_error_bad_args),
		cmocka_unit_test(test_hsm_add_slot_happy_path),
		cmocka_unit_test(test_hsm_add_slot_error_already_exists),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
