#include "bearclave/pkcs11.h"

CK_RV C_SeedRandom(CK_SESSION_HANDLE hSession, CK_BYTE_PTR pSeed,
		   CK_ULONG ulSeedLen) {
	(void)hSession;
	(void)pSeed;
	(void)ulSeedLen;
	return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV C_GenerateRandom(CK_SESSION_HANDLE hSession, CK_BYTE_PTR RandomData,
		       CK_ULONG ulRandomLen) {
	(void)hSession;
	(void)RandomData;
	(void)ulRandomLen;
	return CKR_FUNCTION_NOT_SUPPORTED;
}