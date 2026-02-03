#include "bearclave/pkcs11.h"

#include <setjmp.h> // NOLINT
#include <stdarg.h> // NOLINT
#include <stddef.h> // NOLINT
#include <stdlib.h>

#include <cmocka.h>

static void test_C_GetFunctionList_success(void **state) {
	(void)state;

	CK_FUNCTION_LIST_PTR pFunctionList = NULL;
	CK_RV rv = C_GetFunctionList(&pFunctionList);

	assert_int_equal(rv, CKR_OK);
	assert_non_null(pFunctionList);
	assert_int_equal(pFunctionList->version.major, 3);
	assert_int_equal(pFunctionList->version.minor, 0);
}

static void test_C_GetFunctionList_null_pointer(void **state) {
	(void)state;

	CK_RV rv = C_GetFunctionList(NULL);
	assert_int_equal(rv, CKR_ARGUMENTS_BAD);
}

int main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_C_GetFunctionList_success),
		cmocka_unit_test(test_C_GetFunctionList_null_pointer),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
