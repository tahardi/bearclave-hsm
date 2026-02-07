// NOLINTBEGIN
#define TEST_SLOT_ID 8
#define TEST_SLOT_DESC "Slot Description"
#define TEST_SLOT_MAN_ID "Slot Man ID"
// NOLINTEND

#include "../src/common.h"
#include "../src/hsm.h"
#include "../src/safe.h"

#include <setjmp.h> // NOLINT
#include <stdarg.h> // NOLINT
#include <stddef.h> // NOLINT
#include <stdlib.h>
#include <string.h>

#include <cmocka.h>
#include <stdio.h>

// Helper Functions
static void setup_hsm(void) {
	int err = hsm_initialize();
	assert_int_equal(err, HSM_OK);
	assert_true(hsm_is_initialized());
}

static void setup_slot(slot_t **slot) {
	unsigned char desc[SLOT_DESC_LEN];
	unsigned char man_id[MAN_ID_LEN];

	int err = safe_memcpy_with_padding(desc, SLOT_DESC_LEN, TEST_SLOT_DESC,
					   strlen(TEST_SLOT_DESC), PAD_VAL);
	assert_int_equal(err, SAFE_OK);

	err = safe_memcpy_with_padding(man_id, MAN_ID_LEN, TEST_SLOT_MAN_ID,
				       strlen(TEST_SLOT_MAN_ID), PAD_VAL);
	assert_int_equal(err, SAFE_OK);

	*slot = slot_new(TEST_SLOT_ID, desc, man_id);
	assert_non_null(*slot);
}

static void teardown_hsm(void) {
	int err = hsm_finalize();
	assert_int_equal(err, HSM_OK);
	assert_false(hsm_is_initialized());
}

static void teardown_slot(slot_t *slot) {
	slot_free(slot);
}

// Test Functions
static void test_hsm_initialize_happy_path(void **state) {
	// given
	(void)state;

	// when
	int err = hsm_initialize();

	// then
	assert_int_equal(err, HSM_OK);
	assert_true(hsm_is_initialized());
}

static void test_hsm_finalize_happy_path(void **state) {
	// given
	(void)state;
	setup_hsm();

	// when
	int err = hsm_finalize();

	// then
	assert_int_equal(err, HSM_OK);
	assert_false(hsm_is_initialized());
}

static void test_hsm_finalize_error_not_initialized(void **state) {
	// given
	(void)state;

	// when
	int err = hsm_finalize();

	// then
	assert_int_equal(err, HSM_ERR_NOT_INITIALIZED);
}

static void test_hsm_get_info_happy_path(void **state) {
	// given
	(void)state;
	setup_hsm();

	// when
	hsm_info_t hsm_info;
	int err = hsm_get_info(&hsm_info);

	// then
	assert_int_equal(err, HSM_OK);
	assert_int_equal(hsm_info.ck_version.major, CK_VERSION_MAJOR);
	assert_int_equal(hsm_info.ck_version.minor, CK_VERSION_MINOR);
	assert_int_equal(hsm_info.lib_version.major, LIB_VERSION_MAJOR);
	assert_int_equal(hsm_info.lib_version.minor, LIB_VERSION_MINOR);
	assert_int_equal(hsm_info.flags, FLAGS);
	assert_memory_equal(hsm_info.lib_desc, LIB_DESC, strlen(LIB_DESC));
	assert_memory_equal(hsm_info.man_id, MAN_ID, strlen(MAN_ID));
	teardown_hsm();
}

static void test_hsm_get_info_error_bad_args(void **state) {
	// given
	(void)state;
	setup_hsm();

	// when
	int err = hsm_get_info(NULL);

	// then
	assert_int_equal(err, HSM_ERR_BAD_ARGS);
	teardown_hsm();
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

static void test_hsm_add_slot_happy_path(void **state) {
	// given
	(void)state;
	setup_hsm();

	slot_t *slot;
	setup_slot(&slot);

	unsigned long slots_len = 0;
	int err = hsm_get_slots_len(false, &slots_len);
	assert_int_equal(err, HSM_OK);
	assert_int_equal(slots_len, 0);

	// when
	err = hsm_add_slot(slot);

	// then
	assert_int_equal(err, HSM_OK);

	err = hsm_get_slots_len(false, &slots_len);
	assert_int_equal(err, HSM_OK);
	assert_int_equal(slots_len, 1);
	teardown_hsm();
	teardown_slot(slot);
}

static void test_hsm_add_slot_error_bad_args(void **state) {
	// given
	(void)state;
	setup_hsm();

	// when
	int err = hsm_add_slot(NULL);

	// then
	assert_int_equal(err, HSM_ERR_BAD_ARGS);
	teardown_hsm();
}

static void test_hsm_add_slot_error_not_initialized(void **state) {
	// given
	(void)state;

	slot_t *slot;
	setup_slot(&slot);

	// when
	int err = hsm_add_slot(slot);

	// then
	assert_int_equal(err, HSM_ERR_NOT_INITIALIZED);

	teardown_slot(slot);
}

static void test_hsm_add_slot_error_already_exists(void **state) {
	// given
	(void)state;
	setup_hsm();

	slot_t *slot;
	setup_slot(&slot);

	unsigned long slots_len = 0;
	int err = hsm_get_slots_len(false, &slots_len);
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

	teardown_hsm();
	teardown_slot(slot);
}

static void test_hsm_get_slot_happy_path(void **state) {
	// given
	(void)state;

	setup_hsm();

	slot_t *want;
	setup_slot(&want);

	int err = hsm_add_slot(want);
	assert_int_equal(err, HSM_OK);

	// when
	slot_t *got;
	err = hsm_get_slot(TEST_SLOT_ID, &got);

	// then
	assert_int_equal(err, HSM_OK);
	assert_ptr_equal(got, want);

	teardown_hsm();
	teardown_slot(want);
}

static void test_hsm_get_slot_error_bad_args(void **state) {
	// given
	(void)state;
	setup_hsm();

	// when
	int err = hsm_get_slot(TEST_SLOT_ID, NULL);

	// then
	assert_int_equal(err, HSM_ERR_BAD_ARGS);
	teardown_hsm();
}

static void test_hsm_get_slot_error_not_initialized(void **state) {
	// given
	(void)state;

	// when
	slot_t *slot;
	int err = hsm_get_slot(TEST_SLOT_ID, &slot);

	// then
	assert_int_equal(err, HSM_ERR_NOT_INITIALIZED);
}

static void test_hsm_get_slot_error_not_found(void **state) {
	// given
	(void)state;

	setup_hsm();

	// when
	slot_t *slot;
	int err = hsm_get_slot(TEST_SLOT_ID, &slot);

	// then
	assert_int_equal(err, HSM_ERR_SLOT_NOT_FOUND);

	teardown_hsm();
}

static void test_hsm_get_slots_len_happy_path(void **state) {
	// given
	(void)state;

	setup_hsm();

	slot_t *slot;
	setup_slot(&slot);

	int err = hsm_add_slot(slot);
	assert_int_equal(err, HSM_OK);

	// when
	unsigned long slots_len;
	err = hsm_get_slots_len(false, &slots_len);

	// then
	assert_int_equal(err, HSM_OK);
	assert_ptr_equal(slots_len, 1);

	teardown_hsm();
	teardown_slot(slot);
}

static void test_hsm_get_slots_len_error_bad_args(void **state) {
	// given
	(void)state;
	setup_hsm();

	// when
	int err = hsm_get_slots_len(false, NULL);

	// then
	assert_int_equal(err, HSM_ERR_BAD_ARGS);
	teardown_hsm();
}

static void test_hsm_get_slots_len_error_not_initialized(void **state) {
	// given
	(void)state;

	// when
	unsigned long slots_len = 0;
	int err = hsm_get_slots_len(false, &slots_len);

	// then
	assert_int_equal(err, HSM_ERR_NOT_INITIALIZED);
}

static void test_hsm_get_slot_list_happy_path(void **state) {
	// given
	(void)state;

	setup_hsm();

	slot_t *slot;
	setup_slot(&slot);

	int err = hsm_add_slot(slot);
	assert_int_equal(err, HSM_OK);

	// when
	unsigned long slot_list[1];
	err = hsm_get_slot_list(false, slot_list, 1);

	// then
	assert_int_equal(err, HSM_OK);
	assert_int_equal(slot_list[0], TEST_SLOT_ID);

	teardown_hsm();
	teardown_slot(slot);
}

static void test_hsm_get_slot_list_error_bad_args(void **state) {
	// given
	(void)state;
	setup_hsm();

	// when
	int err = hsm_get_slot_list(false, NULL, 0);

	// then
	assert_int_equal(err, HSM_ERR_BAD_ARGS);
	teardown_hsm();
}

static void test_hsm_get_slot_list_error_not_initialized(void **state) {
	// given
	(void)state;

	// when
	unsigned long slot_list[1];
	int err = hsm_get_slot_list(false, slot_list, 0);

	// then
	assert_int_equal(err, HSM_ERR_NOT_INITIALIZED);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_hsm_initialize_happy_path),

		cmocka_unit_test(test_hsm_finalize_happy_path),
		cmocka_unit_test(test_hsm_finalize_error_not_initialized),

		cmocka_unit_test(test_hsm_get_info_happy_path),
		cmocka_unit_test(test_hsm_get_info_error_bad_args),
		cmocka_unit_test(test_hsm_get_info_error_not_initialized),

		cmocka_unit_test(test_hsm_add_slot_happy_path),
		cmocka_unit_test(test_hsm_add_slot_error_bad_args),
		cmocka_unit_test(test_hsm_add_slot_error_not_initialized),
		cmocka_unit_test(test_hsm_add_slot_error_already_exists),

		cmocka_unit_test(test_hsm_get_slot_happy_path),
		cmocka_unit_test(test_hsm_get_slot_error_bad_args),
		cmocka_unit_test(test_hsm_get_slot_error_not_initialized),
		cmocka_unit_test(test_hsm_get_slot_error_not_found),

		cmocka_unit_test(test_hsm_get_slots_len_happy_path),
		cmocka_unit_test(test_hsm_get_slots_len_error_bad_args),
		cmocka_unit_test(test_hsm_get_slots_len_error_not_initialized),

		cmocka_unit_test(test_hsm_get_slot_list_happy_path),
		cmocka_unit_test(test_hsm_get_slot_list_error_bad_args),
		cmocka_unit_test(test_hsm_get_slot_list_error_not_initialized),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
