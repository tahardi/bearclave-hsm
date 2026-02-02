// NOLINTBEGIN(readability-non-const-parameter,bugprone-easily-swappable-parameters)
#include "bearclave/pkcs11.h"

CK_RV C_Initialize(CK_VOID_PTR pInitArgs) {
	(void)pInitArgs;
	return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV C_Finalize(CK_VOID_PTR pReserved) {
	(void)pReserved;
	return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV C_GetInfo(CK_INFO_PTR pInfo) {
	(void)pInfo;
	return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV C_GetFunctionList(CK_FUNCTION_LIST_PTR_PTR ppFunctionList) {
	(void)ppFunctionList;
	return CKR_FUNCTION_NOT_SUPPORTED;
}
// NOLINTEND(readability-non-const-parameter,bugprone-easily-swappable-parameters)
