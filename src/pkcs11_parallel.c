#include "bearclave/pkcs11.h"

CK_RV C_GetFunctionStatus(CK_SESSION_HANDLE hSession) {
	(void)hSession;
	return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV C_CancelFunction(CK_SESSION_HANDLE hSession) {
	(void)hSession;
	return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV C_WaitForSlotEvent(CK_FLAGS flags, CK_SLOT_ID_PTR pSlot,
			 CK_VOID_PTR pReserved) {
	(void)flags;
	(void)pSlot;
	(void)pReserved;
	return CKR_FUNCTION_NOT_SUPPORTED;
}
