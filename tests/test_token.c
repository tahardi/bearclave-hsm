// NOLINTBEGIN
#define TEST_TOKEN_FW_VERSION_MAJOR 0
#define TEST_TOKEN_FW_VERSION_MINOR 1
#define TEST_TOKEN_HW_VERSION_MAJOR 0
#define TEST_TOKEN_HW_VERSION_MINOR 1
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

static void setup(token_t **token) {
	unsigned char man_id[MAN_ID_LEN];
	unsigned char model[TOKEN_MODEL_LEN];
	unsigned char serial[TOKEN_SERIAL_LEN];
	version_t fw_version = {TEST_TOKEN_FW_VERSION_MAJOR, TEST_TOKEN_FW_VERSION_MINOR};
	version_t hw_version = {TEST_TOKEN_HW_VERSION_MAJOR, TEST_TOKEN_HW_VERSION_MINOR};

	int err = safe_memcpy_with_padding(man_id, MAN_ID_LEN, TEST_TOKEN_MAN_ID, strlen(TEST_TOKEN_MAN_ID), PAD_VAL);
	assert_int_equal(err, SAFE_OK);

	err = safe_memcpy_with_padding(model, TOKEN_MODEL_LEN, TEST_TOKEN_MODEL, strlen(TEST_TOKEN_MODEL), PAD_VAL);
	assert_int_equal(err, SAFE_OK);

	err = safe_memcpy_with_padding(serial, TOKEN_SERIAL_LEN, TEST_TOKEN_SERIAL, strlen(TEST_TOKEN_SERIAL), PAD_VAL);
	assert_int_equal(err, SAFE_OK);

	*token = token_new(man_id, model, serial, fw_version, hw_version);
	assert_non_null(*token);
}

static void setup_and_initialize(token_t **token) {
	setup(token);

	unsigned char label[TOKEN_LABEL_LEN];
	int err = safe_memcpy_with_padding(label, TOKEN_LABEL_LEN, TEST_TOKEN_LABEL, strlen(TEST_TOKEN_LABEL), PAD_VAL);
	assert_int_equal(err, SAFE_OK);

	unsigned char *pin = (unsigned char *)TEST_TOKEN_PIN;
	unsigned long pin_len = strlen(TEST_TOKEN_PIN);
	err = token_initialize(*token, label, pin, pin_len);
	assert_int_equal(err, TOKEN_OK);
}

static void teardown(token_t *token) {
	token_free(token);
}

static void test_token_initialize_happy_path(void **state) {
	// given
	(void)state;

	token_t *token;
	setup(&token);

	unsigned char label[TOKEN_LABEL_LEN];
	int err = safe_memcpy_with_padding(label, TOKEN_LABEL_LEN, TEST_TOKEN_LABEL, strlen(TEST_TOKEN_LABEL), PAD_VAL);
	assert_int_equal(err, SAFE_OK);

	unsigned char *pin = (unsigned char *)TEST_TOKEN_PIN;
	unsigned long pin_len = strlen(TEST_TOKEN_PIN);

	// when
	err = token_initialize(token, label, pin, pin_len);

	// then
	assert_int_equal(err, TOKEN_OK);

	unsigned char *so_pin;
	unsigned long so_pin_len;
	err = token_get_so_pin(token, &so_pin, &so_pin_len);
	assert_int_equal(err, TOKEN_OK);
	assert_memory_equal(so_pin, TEST_TOKEN_PIN, strlen(TEST_TOKEN_PIN));
	teardown(token);
}

static void test_token_initialize_happy_path_pin_changes(void **state) {
	// given
	(void)state;

	token_t *token;
	setup_and_initialize(&token);

	unsigned char *old_pin;
	unsigned long old_pin_len;
	int err = token_get_so_pin(token, &old_pin, &old_pin_len);
	assert_int_equal(err, TOKEN_OK);
	assert_memory_equal(old_pin, TEST_TOKEN_PIN, strlen(TEST_TOKEN_PIN));

	unsigned char label[TOKEN_LABEL_LEN];
	int err = safe_memcpy_with_padding(label, TOKEN_LABEL_LEN, TEST_TOKEN_LABEL, strlen(TEST_TOKEN_LABEL), PAD_VAL);
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
	teardown(token);
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

	// when

	// then
}

static void test_token_get_info_error_not_initialized(void **state) {
	// given
	(void)state;

	// when

	// then
}

static void test_token_get_info_error_bad_args(void **state) {
	// given
	(void)state;

	// when

	// then
}

static void test_token_add_mech_happy_path(void **state) {
	// given
	(void)state;

	// when

	// then
}

static void test_token_add_mech_error_already_exists(void **state) {
	// given
	(void)state;

	// when

	// then
}

static void test_token_get_mech_happy_path(void **state) {
	// given
	(void)state;

	// when

	// then
}

static void test_token_get_mechs_len_happy_path(void **state) {
	// given
	(void)state;

	// when

	// then
}

static void test_token_get_mech_list_happy_path(void **state) {
	// given
	(void)state;

	// when

	// then
}

static void test_token_get_so_pin_happy_path(void **state) {
	// given
	(void)state;

	// when

	// then
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
		cmocka_unit_test(test_token_add_mech_error_already_exists),
		cmocka_unit_test(test_token_get_mech_happy_path),
		cmocka_unit_test(test_token_get_mechs_len_happy_path),
		cmocka_unit_test(test_token_get_mech_list_happy_path),
		cmocka_unit_test(test_token_get_so_pin_happy_path),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
