// NOLINTBEGIN
#define TEST_TOKEN_FW_VERSION_MAJOR 0
#define TEST_TOKEN_FW_VERSION_MINOR 1
#define TEST_TOKEN_HW_VERSION_MAJOR 0
#define TEST_TOKEN_HW_VERSION_MINOR 1
#define TEST_TOKEN_LABEL "Token Label"
#define TEST_TOKEN_MAN_ID "Token ID"
#define TEST_TOKEN_MECH_TYPE 8
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
static void setup_mech(mech_t **mech) {
	*mech = mech_new(TEST_TOKEN_MECH_TYPE);
	assert_non_null(*mech);
}

static void setup_token(token_t **token) {
	unsigned char man_id[MAN_ID_LEN];
	unsigned char model[TOKEN_MODEL_LEN];
	unsigned char serial[TOKEN_SERIAL_LEN];
	version_t fw_version = {TEST_TOKEN_FW_VERSION_MAJOR,
				TEST_TOKEN_FW_VERSION_MINOR};
	version_t hw_version = {TEST_TOKEN_HW_VERSION_MAJOR,
				TEST_TOKEN_HW_VERSION_MINOR};

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

	*token = token_new(man_id, model, serial, fw_version, hw_version);
	assert_non_null(*token);
}

static void setup_and_initialize_token(token_t **token) {
	setup_token(token);

	unsigned char label[TOKEN_LABEL_LEN];
	int err = safe_memcpy_with_padding(label, TOKEN_LABEL_LEN,
					   TEST_TOKEN_LABEL,
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

static void teardown_mech(mech_t *mech) {
	mech_free(mech);
}

static void teardown_token(token_t *token) {
	token_free(token);
}

// Test Functions
static void test_token_initialize_happy_path(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_token(&token);

	unsigned char label[TOKEN_LABEL_LEN];
	int err = safe_memcpy_with_padding(label, TOKEN_LABEL_LEN,
					   TEST_TOKEN_LABEL,
					   strlen(TEST_TOKEN_LABEL), PAD_VAL);
	assert_int_equal(err, SAFE_OK);

	unsigned char *pin = malloc(strlen(TEST_TOKEN_PIN));
	unsigned long pin_len = strlen(TEST_TOKEN_PIN);
	err = safe_memcpy(pin, pin_len, TEST_TOKEN_PIN, strlen(TEST_TOKEN_PIN));
	assert_int_equal(err, SAFE_OK);

	// when
	err = token_initialize(token, label, pin, pin_len);

	// then
	assert_int_equal(err, TOKEN_OK);

	unsigned char *so_pin;
	unsigned long so_pin_len;
	err = token_get_so_pin(token, &so_pin, &so_pin_len);
	assert_int_equal(err, TOKEN_OK);
	assert_memory_equal(so_pin, TEST_TOKEN_PIN, strlen(TEST_TOKEN_PIN));
	teardown_token(token);
	free(pin);
}

static void test_token_initialize_happy_path_pin_changes(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_and_initialize_token(&token);

	unsigned char *old_pin;
	unsigned long old_pin_len;
	int err = token_get_so_pin(token, &old_pin, &old_pin_len);
	assert_int_equal(err, TOKEN_OK);
	assert_memory_equal(old_pin, TEST_TOKEN_PIN, strlen(TEST_TOKEN_PIN));

	unsigned char label[TOKEN_LABEL_LEN];
	err = safe_memcpy_with_padding(label, TOKEN_LABEL_LEN, TEST_TOKEN_LABEL,
				       strlen(TEST_TOKEN_LABEL), PAD_VAL);
	assert_int_equal(err, SAFE_OK);

	unsigned char *new_pin = (unsigned char *)"new pin";
	unsigned long new_pin_len = strlen("new pin");

	// when
	err = token_initialize(token, label, new_pin, new_pin_len);

	// then
	assert_int_equal(err, TOKEN_OK);

	unsigned char *so_pin;
	unsigned long so_pin_len;
	err = token_get_so_pin(token, &so_pin, &so_pin_len);
	assert_int_equal(err, TOKEN_OK);
	assert_memory_equal(so_pin, new_pin, new_pin_len);
	teardown_token(token);
}

static void test_token_initialize_error_bad_args(void **state) {
	// given
	(void)state;

	// when
	int err = token_initialize(NULL, NULL, NULL, 0);

	// then
	assert_int_equal(err, TOKEN_ERR_BAD_ARGS);
}

static void test_token_get_info_happy_path(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_and_initialize_token(&token);

	// when
	token_info_t info;
	int err = token_get_info(token, &info);

	// then
	assert_int_equal(err, TOKEN_OK);
	assert_int_equal(info.ulMaxSessionCount, TOKEN_MAX_SESSIONS);
	assert_int_equal(info.ulMaxRwSessionCount, TOKEN_MAX_SESSIONS_RW);
	assert_int_equal(info.ulMaxPinLen, TOKEN_MAX_PIN_LEN);
	assert_int_equal(info.ulMinPinLen, TOKEN_MIN_PIN_LEN);
	assert_int_equal(info.ulTotalPublicMemory, TOKEN_TOTAL_PUBLIC_MEMORY);
	assert_int_equal(info.ulTotalPrivateMemory, TOKEN_TOTAL_PRIVATE_MEMORY);
	assert_int_equal(info.fw_version.major, TEST_TOKEN_FW_VERSION_MAJOR);
	assert_int_equal(info.fw_version.minor, TEST_TOKEN_FW_VERSION_MINOR);
	assert_int_equal(info.hw_version.major, TEST_TOKEN_HW_VERSION_MAJOR);
	assert_int_equal(info.hw_version.minor, TEST_TOKEN_HW_VERSION_MINOR);
	assert_memory_equal(info.label, TEST_TOKEN_LABEL,
			    strlen(TEST_TOKEN_LABEL));
	assert_memory_equal(info.man_id, TEST_TOKEN_MAN_ID,
			    strlen(TEST_TOKEN_MAN_ID));
	assert_memory_equal(info.model, TEST_TOKEN_MODEL,
			    strlen(TEST_TOKEN_MODEL));
	assert_memory_equal(info.serial, TEST_TOKEN_SERIAL,
			    strlen(TEST_TOKEN_SERIAL));
	teardown_token(token);
}

static void test_token_get_info_error_not_initialized(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_token(&token);

	// when
	token_info_t info;
	int err = token_get_info(token, &info);

	// then
	assert_int_equal(err, TOKEN_ERR_NOT_INITIALIZED);
	teardown_token(token);
}

static void test_token_get_info_error_bad_args(void **state) {
	// given
	(void)state;

	// when
	int err = token_get_info(NULL, NULL);

	// then
	assert_int_equal(err, TOKEN_ERR_BAD_ARGS);
}

static void test_token_add_mech_happy_path(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_and_initialize_token(&token);

	mech_t *mech;
	setup_mech(&mech);

	unsigned long mechs_len;
	int err = token_get_mechs_len(token, &mechs_len);
	assert_int_equal(err, TOKEN_OK);
	assert_int_equal(mechs_len, 0);

	// when
	err = token_add_mech(token, mech);

	// then
	assert_int_equal(err, TOKEN_OK);
	err = token_get_mechs_len(token, &mechs_len);
	assert_int_equal(err, TOKEN_OK);
	assert_int_equal(mechs_len, 1);
	teardown_mech(mech);
	teardown_token(token);
}

static void test_token_add_mech_error_bad_args(void **state) {
	// given
	(void)state;

	// when
	int err = token_add_mech(NULL, NULL);

	// then
	assert_int_equal(err, TOKEN_ERR_BAD_ARGS);
}

static void test_token_add_mech_error_not_initialized(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_token(&token);

	mech_t *mech;
	setup_mech(&mech);

	// when
	int err = token_add_mech(token, mech);

	// then
	assert_int_equal(err, TOKEN_ERR_NOT_INITIALIZED);
	teardown_mech(mech);
	teardown_token(token);
}

static void test_token_add_mech_error_already_exists(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_and_initialize_token(&token);

	mech_t *mech;
	setup_mech(&mech);

	int err = token_add_mech(token, mech);
	assert_int_equal(err, TOKEN_OK);

	unsigned long mechs_len;
	err = token_get_mechs_len(token, &mechs_len);
	assert_int_equal(err, TOKEN_OK);
	assert_int_equal(mechs_len, 1);

	// when
	err = token_add_mech(token, mech);

	// then
	assert_int_equal(err, TOKEN_ERR_MECH_ALREADY_EXISTS);
	teardown_mech(mech);
	teardown_token(token);
}

static void test_token_get_mech_happy_path(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_and_initialize_token(&token);

	mech_t *want;
	setup_mech(&want);

	int err = token_add_mech(token, want);
	assert_int_equal(err, TOKEN_OK);

	// when
	mech_t *got;
	err = token_get_mech(token, TEST_TOKEN_MECH_TYPE, &got);

	// then
	assert_int_equal(err, TOKEN_OK);
	assert_ptr_equal(got, want);
	teardown_mech(want);
	teardown_token(token);
}

static void test_token_get_mech_error_bad_args(void **state) {
	// given
	(void)state;

	// when
	int err = token_get_mech(NULL, 0, NULL);

	// then
	assert_int_equal(err, TOKEN_ERR_BAD_ARGS);
}

static void test_token_get_mech_error_not_initialized(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_token(&token);

	// when
	mech_t *mech;
	int err = token_get_mech(token, 0, &mech);

	// then
	assert_int_equal(err, TOKEN_ERR_NOT_INITIALIZED);
	teardown_token(token);
}

static void test_token_get_mechs_len_happy_path(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_and_initialize_token(&token);

	mech_t *mech;
	setup_mech(&mech);

	int err = token_add_mech(token, mech);
	assert_int_equal(err, TOKEN_OK);

	// when
	unsigned long mechs_len;
	err = token_get_mechs_len(token, &mechs_len);

	// then
	assert_int_equal(err, TOKEN_OK);
	assert_int_equal(mechs_len, 1);
	teardown_mech(mech);
	teardown_token(token);
}

static void test_token_get_mechs_len_error_bad_args(void **state) {
	// given
	(void)state;

	// when
	int err = token_get_mechs_len(NULL, NULL);

	// then
	assert_int_equal(err, TOKEN_ERR_BAD_ARGS);
}

static void test_token_get_mechs_len_error_not_initialized(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_token(&token);

	// when
	unsigned long mechs_len;
	int err = token_get_mechs_len(token, &mechs_len);

	// then
	assert_int_equal(err, TOKEN_ERR_NOT_INITIALIZED);
	teardown_token(token);
}

static void test_token_get_mech_list_happy_path(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_and_initialize_token(&token);

	mech_t *mech;
	setup_mech(&mech);

	int err = token_add_mech(token, mech);
	assert_int_equal(err, TOKEN_OK);

	// when
	unsigned long mechs_list[1];
	err = token_get_mech_list(token, mechs_list, 1);

	// then
	assert_int_equal(err, TOKEN_OK);
	assert_int_equal(mechs_list[0], TEST_TOKEN_MECH_TYPE);
	teardown_mech(mech);
	teardown_token(token);
}

static void test_token_get_mech_list_error_bad_args(void **state) {
	// given
	(void)state;

	// when
	int err = token_get_mech_list(NULL, NULL, 0);

	// then
	assert_int_equal(err, TOKEN_ERR_BAD_ARGS);
}

static void test_token_get_mech_list_error_not_initialized(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_token(&token);

	// when
	unsigned long mechs_list[1];
	int err = token_get_mech_list(token, mechs_list, 1);

	// then
	assert_int_equal(err, TOKEN_ERR_NOT_INITIALIZED);
	teardown_token(token);
}

static void test_token_get_mech_list_error_overflow(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_and_initialize_token(&token);

	// when
	unsigned long mechs_list[1];
	int err = token_get_mech_list(token, mechs_list, 1);

	// then
	assert_int_equal(err, TOKEN_ERR_OVERFLOW);
	teardown_token(token);
}

static void test_token_get_so_pin_happy_path(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_and_initialize_token(&token);

	// when
	unsigned char *so_pin;
	unsigned long so_pin_len;
	int err = token_get_so_pin(token, &so_pin, &so_pin_len);

	// then
	assert_int_equal(err, TOKEN_OK);
	assert_memory_equal(so_pin, TEST_TOKEN_PIN, strlen(TEST_TOKEN_PIN));
	teardown_token(token);
}

static void test_token_get_so_pin_error_bad_args(void **state) {
	// given
	(void)state;

	// when
	int err = token_get_so_pin(NULL, NULL, NULL);

	// then
	assert_int_equal(err, TOKEN_ERR_BAD_ARGS);
}

static void test_token_get_so_pin_error_not_initialized(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_token(&token);

	// when
	unsigned char *pin;
	unsigned long pin_len;
	int err = token_get_so_pin(token, &pin, &pin_len);

	// then
	assert_int_equal(err, TOKEN_ERR_NOT_INITIALIZED);
	teardown_token(token);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_token_initialize_happy_path),
		cmocka_unit_test(test_token_initialize_happy_path_pin_changes),
		cmocka_unit_test(test_token_initialize_error_bad_args),

		cmocka_unit_test(test_token_get_info_happy_path),
		cmocka_unit_test(test_token_get_info_error_not_initialized),
		cmocka_unit_test(test_token_get_info_error_bad_args),

		cmocka_unit_test(test_token_add_mech_happy_path),
		cmocka_unit_test(test_token_add_mech_error_bad_args),
		cmocka_unit_test(test_token_add_mech_error_not_initialized),
		cmocka_unit_test(test_token_add_mech_error_already_exists),

		cmocka_unit_test(test_token_get_mech_happy_path),
		cmocka_unit_test(test_token_get_mech_error_bad_args),
		cmocka_unit_test(test_token_get_mech_error_not_initialized),

		cmocka_unit_test(test_token_get_mechs_len_happy_path),
		cmocka_unit_test(test_token_get_mechs_len_error_bad_args),
		cmocka_unit_test(
			test_token_get_mechs_len_error_not_initialized),

		cmocka_unit_test(test_token_get_mech_list_happy_path),
		cmocka_unit_test(test_token_get_mech_list_error_bad_args),
		cmocka_unit_test(
			test_token_get_mech_list_error_not_initialized),
		cmocka_unit_test(test_token_get_mech_list_error_overflow),

		cmocka_unit_test(test_token_get_so_pin_happy_path),
		cmocka_unit_test(test_token_get_so_pin_error_bad_args),
		cmocka_unit_test(test_token_get_so_pin_error_not_initialized),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
