#include "../src/hsm.h"
#include "bearclave/pkcs11.h"

#include <setjmp.h> // NOLINT
#include <stdarg.h> // NOLINT
#include <stddef.h> // NOLINT
#include <stdlib.h>
#include <string.h>

#include <cmocka.h>

static CK_FUNCTION_LIST_PTR get_function_list_ptr(void) {
	CK_FUNCTION_LIST_PTR pFunctionList = NULL;
	CK_RV rv = C_GetFunctionList(&pFunctionList);

	assert_int_equal(rv, CKR_OK);
	assert_non_null(pFunctionList);
	assert_int_equal(pFunctionList->version.major, 3);
	assert_int_equal(pFunctionList->version.minor, 0);
	return pFunctionList;
}

static void teardown(void) {
	CK_FUNCTION_LIST_PTR pFunctionList = get_function_list_ptr();
	CK_RV rv = pFunctionList->C_Finalize(NULL);
	assert_int_equal(rv, CKR_OK);
}

static void test_C_Initialize_happy_path(void **state) {
	// given
	(void)state;
	CK_FUNCTION_LIST_PTR pFunctionList = get_function_list_ptr();

	CK_VOID_PTR pInitArgs = NULL;

	// when
	CK_RV rv = pFunctionList->C_Initialize(pInitArgs);

	// then
	assert_int_equal(rv, CKR_OK);
}

static void test_C_Finalize_happy_path(void **state) {
	// given
	(void)state;
	CK_FUNCTION_LIST_PTR pFunctionList = get_function_list_ptr();

	CK_VOID_PTR pInitArgs = NULL;
	CK_RV rv = pFunctionList->C_Initialize(pInitArgs);
	assert_int_equal(rv, CKR_OK);

	CK_VOID_PTR pReserved = NULL;

	// when
	rv = pFunctionList->C_Finalize(pReserved);

	// then
	assert_int_equal(rv, CKR_OK);
}

static void test_C_Finalize_error_not_initialized(void **state) {
	// given
	(void)state;
	CK_FUNCTION_LIST_PTR pFunctionList = get_function_list_ptr();

	CK_VOID_PTR pReserved = NULL;

	// when
	CK_RV rv = pFunctionList->C_Finalize(pReserved);

	// then
	assert_int_equal(rv, CKR_DEVICE_ERROR);
}

static void test_C_GetInfo_happy_path(void **state) {
	// given
	(void)state;
	CK_FUNCTION_LIST_PTR pFunctionList = get_function_list_ptr();

	CK_VOID_PTR pInitArgs = NULL;
	CK_RV rv = pFunctionList->C_Initialize(pInitArgs);
	assert_int_equal(rv, CKR_OK);

	CK_INFO info;

	// when
	rv = pFunctionList->C_GetInfo(&info);

	// then
	assert_int_equal(rv, CKR_OK);
	assert_int_equal(info.cryptokiVersion.major, g_ck_version_major);
	assert_int_equal(info.cryptokiVersion.minor, g_ck_version_minor);
	assert_int_equal(info.libraryVersion.major, g_lib_version_major);
	assert_int_equal(info.libraryVersion.minor, g_lib_version_minor);
	assert_int_equal(info.flags, g_flags);
	assert_memory_equal(info.manufacturerID, g_man_id, strlen(g_man_id));
	assert_memory_equal(info.libraryDescription, g_lib_desc,
			    strlen(g_lib_desc));
	teardown();
}

static void test_C_GetInfo_error_not_initialized(void **state) {
	// given
	(void)state;
	CK_FUNCTION_LIST_PTR pFunctionList = get_function_list_ptr();

	CK_INFO info;

	// when
	CK_RV rv = pFunctionList->C_GetInfo(&info);

	// then
	assert_int_equal(rv, CKR_DEVICE_ERROR);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_C_Initialize_happy_path),
		cmocka_unit_test(test_C_Finalize_happy_path),
		cmocka_unit_test(test_C_Finalize_error_not_initialized),
		cmocka_unit_test(test_C_GetInfo_happy_path),
		cmocka_unit_test(test_C_GetInfo_error_not_initialized),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
