// NOLINTBEGIN
#define TEST_SLOT_ID 8
#define TEST_SLOT_DESC "Slot Description"
#define TEST_SLOT_MAN_ID "Slot Man ID"

#define TEST_TOKEN_LABEL "Token Label"
#define TEST_TOKEN_MAN_ID "Token ID"
#define TEST_TOKEN_MODEL "Token Model"
#define TEST_TOKEN_SERIAL "Token Serial"
#define TEST_TOKEN_PIN "Token Pin"
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

static void setup_token(token_t **token) {
	unsigned char man_id[MAN_ID_LEN];
	unsigned char model[TOKEN_MODEL_LEN];
	unsigned char serial[TOKEN_SERIAL_LEN];

	int err =
		safe_memcpy_with_padding(man_id, MAN_ID_LEN, TEST_TOKEN_MAN_ID,
					 strlen(TEST_TOKEN_MAN_ID), PAD_VAL);
	assert_int_equal(err, SAFE_OK);

	err = safe_memcpy_with_padding(model, TOKEN_MODEL_LEN, TEST_TOKEN_MODEL,
				       strlen(TEST_TOKEN_MODEL), PAD_VAL);
	assert_int_equal(err, SAFE_OK);

	err = safe_memcpy_with_padding(serial, TOKEN_SERIAL_LEN,
				       TEST_TOKEN_SERIAL,
				       strlen(TEST_TOKEN_SERIAL), PAD_VAL);
	assert_int_equal(err, SAFE_OK);

	*token = token_new(man_id, model, serial);
	assert_non_null(*token);

	unsigned char label[TOKEN_LABEL_LEN];
	err = safe_memcpy_with_padding(label, TOKEN_LABEL_LEN, TEST_TOKEN_LABEL,
				       strlen(TEST_TOKEN_LABEL), PAD_VAL);
	assert_int_equal(err, SAFE_OK);

	unsigned char *pin = malloc(strlen(TEST_TOKEN_PIN));
	unsigned long pin_len = strlen(TEST_TOKEN_PIN);
	err = safe_memcpy(pin, pin_len, TEST_TOKEN_PIN, strlen(TEST_TOKEN_PIN));
	assert_int_equal(err, SAFE_OK);

	err = token_initialize(*token, label, pin, pin_len);
	assert_int_equal(err, TOKEN_OK);
	free(pin);
}

static void teardown_slot(slot_t *slot) {
	slot_free(slot);
}

static void teardown_token(token_t *token) {
	token_free(token);
}

// Test Functions
static void test_slot_has_token_happy_path_no_token(void **state) {
	// given
	(void)state;

	slot_t *slot;
	setup_slot(&slot);

	// when
	bool has_token = slot_has_token(slot);

	// then
	assert_false(has_token);
	teardown_slot(slot);
}

static void test_slot_has_token_happy_path_token(void **state) {
	// given
	(void)state;

	slot_t *slot;
	setup_slot(&slot);

	token_t *token;
	setup_token(&token);

	int err = slot_set_token(slot, token);
	assert_int_equal(err, SLOT_OK);

	// when
	bool has_token = slot_has_token(slot);

	// then
	assert_true(has_token);
	teardown_slot(slot);
	teardown_token(token);
}

static void test_slot_has_token_happy_path_slot_null(void **state) {
	// given
	(void)state;

	// when
	bool has_token = slot_has_token(NULL);

	// then
	assert_false(has_token);
}

static void test_slot_get_id_happy_path(void **state) {
	// given
	(void)state;

	slot_t *slot;
	setup_slot(&slot);

	// when
	unsigned long slot_id;
	int err = slot_get_id(slot, &slot_id);

	// then
	assert_int_equal(err, SLOT_OK);
	assert_int_equal(slot_id, TEST_SLOT_ID);
	teardown_slot(slot);
}

static void test_slot_get_id_error_bad_args(void **state) {
	// given
	(void)state;

	// when
	int err = slot_get_id(NULL, NULL);

	// then
	assert_int_equal(err, SLOT_ERR_BAD_ARGS);
}

static void test_slot_get_info_happy_path(void **state) {
	// given
	(void)state;

	slot_t *slot;
	setup_slot(&slot);

	// when
	slot_info_t info;
	int err = slot_get_info(slot, &info);

	// then
	assert_int_equal(err, SLOT_OK);
	assert_int_equal(info.fw_version.major, FW_VERSION_MAJOR);
	assert_int_equal(info.fw_version.minor, FW_VERSION_MINOR);
	assert_int_equal(info.hw_version.major, HW_VERSION_MAJOR);
	assert_int_equal(info.hw_version.minor, HW_VERSION_MINOR);
	assert_memory_equal(info.slot_desc, TEST_SLOT_DESC,
			    strlen(TEST_SLOT_DESC));
	assert_memory_equal(info.man_id, TEST_SLOT_MAN_ID,
			    strlen(TEST_SLOT_MAN_ID));
	teardown_slot(slot);
}

static void test_slot_get_info_error_bad_args(void **state) {
	// given
	(void)state;

	// when
	int err = slot_get_info(NULL, NULL);

	// then
	assert_int_equal(err, SLOT_ERR_BAD_ARGS);
}

static void test_slot_get_token_happy_path(void **state) {
	// given
	(void)state;

	slot_t *slot;
	setup_slot(&slot);

	token_t *want;
	setup_token(&want);

	int err = slot_set_token(slot, want);
	assert_int_equal(err, SLOT_OK);

	// when
	token_t *got;
	err = slot_get_token(slot, &got);

	// then
	assert_int_equal(err, SLOT_OK);
	assert_ptr_equal(got, want);

	teardown_slot(slot);
	teardown_token(want);
}

static void test_slot_get_token_error_bad_args(void **state) {
	// given
	(void)state;

	// when
	int err = slot_get_token(NULL, NULL);

	// then
	assert_int_equal(err, SLOT_ERR_BAD_ARGS);
}

static void test_slot_get_token_error_token_not_set(void **state) {
	// given
	(void)state;

	slot_t *slot;
	setup_slot(&slot);

	// when
	token_t *token;
	int err = slot_get_token(slot, &token);

	// then
	assert_int_equal(err, SLOT_ERR_TOKEN_NOT_SET);

	teardown_slot(slot);
}

static void test_slot_set_token_happy_path(void **state) {
	// given
	(void)state;

	slot_t *slot;
	setup_slot(&slot);

	token_t *want;
	setup_token(&want);

	// when
	int err = slot_set_token(slot, want);
	assert_int_equal(err, SLOT_OK);

	// then
	assert_int_equal(err, SLOT_OK);

	token_t *got;
	err = slot_get_token(slot, &got);
	assert_int_equal(err, SLOT_OK);
	assert_ptr_equal(got, want);

	teardown_slot(slot);
	teardown_token(want);
}

static void test_slot_set_token_error_bad_args(void **state) {
	// given
	(void)state;

	// when
	int err = slot_set_token(NULL, NULL);

	// then
	assert_int_equal(err, SLOT_ERR_BAD_ARGS);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_slot_has_token_happy_path_token),
		cmocka_unit_test(test_slot_has_token_happy_path_no_token),
		cmocka_unit_test(test_slot_has_token_happy_path_slot_null),

		cmocka_unit_test(test_slot_get_id_happy_path),
		cmocka_unit_test(test_slot_get_id_error_bad_args),

		cmocka_unit_test(test_slot_get_info_happy_path),
		cmocka_unit_test(test_slot_get_info_error_bad_args),

		cmocka_unit_test(test_slot_get_token_happy_path),
		cmocka_unit_test(test_slot_get_token_error_bad_args),
		cmocka_unit_test(test_slot_get_token_error_token_not_set),

		cmocka_unit_test(test_slot_set_token_happy_path),
		cmocka_unit_test(test_slot_set_token_error_bad_args),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
