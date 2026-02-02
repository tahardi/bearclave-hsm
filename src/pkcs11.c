#include "bearclave_pkcs11.h"

static CK_FUNCTION_LIST g_function_list_3_0 = {
	.version = {.major = 3, .minor = 0},

	// General
	.C_Initialize = C_Initialize,
	.C_Finalize = C_Finalize,
	.C_GetInfo = C_GetInfo,
	.C_GetFunctionList = C_GetFunctionList,

	// Slot and Token management
	.C_GetSlotList = C_GetSlotList,
	.C_GetSlotInfo = C_GetSlotInfo,
	.C_GetTokenInfo = C_GetTokenInfo,
	.C_GetMechanismList = C_GetMechanismList,
	.C_GetMechanismInfo = C_GetMechanismInfo,
	.C_InitToken = C_InitToken,
	.C_InitPIN = C_InitPIN,
	.C_SetPIN = C_SetPIN,

	// Session management
	.C_OpenSession = C_OpenSession,
	.C_CloseSession = C_CloseSession,
	.C_CloseAllSessions = C_CloseAllSessions,
	.C_GetSessionInfo = C_GetSessionInfo,
	.C_GetOperationState = C_GetOperationState,
	.C_SetOperationState = C_SetOperationState,
	.C_Login = C_Login,
	.C_Logout = C_Logout,

	// Object management
	.C_CreateObject = C_CreateObject,
	.C_CopyObject = C_CopyObject,
	.C_DestroyObject = C_DestroyObject,
	.C_GetObjectSize = C_GetObjectSize,
	.C_GetAttributeValue = C_GetAttributeValue,
	.C_SetAttributeValue = C_SetAttributeValue,
	.C_FindObjectsInit = C_FindObjectsInit,
	.C_FindObjects = C_FindObjects,
	.C_FindObjectsFinal = C_FindObjectsFinal,

	// Encrypt
	.C_EncryptInit = C_EncryptInit,
	.C_Encrypt = C_Encrypt,
	.C_EncryptUpdate = C_EncryptUpdate,
	.C_EncryptFinal = C_EncryptFinal,

	// Decrypt
	.C_DecryptInit = C_DecryptInit,
	.C_Decrypt = C_Decrypt,
	.C_DecryptUpdate = C_DecryptUpdate,
	.C_DecryptFinal = C_DecryptFinal,

	// Digest
	.C_DigestInit = C_DigestInit,
	.C_Digest = C_Digest,
	.C_DigestUpdate = C_DigestUpdate,
	.C_DigestKey = C_DigestKey,
	.C_DigestFinal = C_DigestFinal,

	// Sign
	.C_SignInit = C_SignInit,
	.C_Sign = C_Sign,
	.C_SignUpdate = C_SignUpdate,
	.C_SignFinal = C_SignFinal,
	.C_SignRecoverInit = C_SignRecoverInit,
	.C_SignRecover = C_SignRecover,

	// Verify
	.C_VerifyInit = C_VerifyInit,
	.C_Verify = C_Verify,
	.C_VerifyUpdate = C_VerifyUpdate,
	.C_VerifyFinal = C_VerifyFinal,
	.C_VerifyRecoverInit = C_VerifyRecoverInit,
	.C_VerifyRecover = C_VerifyRecover,

	// Dual-function crypto
	.C_DigestEncryptUpdate = C_DigestEncryptUpdate,
	.C_DecryptDigestUpdate = C_DecryptDigestUpdate,
	.C_SignEncryptUpdate = C_SignEncryptUpdate,
	.C_DecryptVerifyUpdate = C_DecryptVerifyUpdate,

	// Key Management
	.C_GenerateKey = C_GenerateKey,
	.C_GenerateKeyPair = C_GenerateKeyPair,
	.C_WrapKey = C_WrapKey,
	.C_UnwrapKey = C_UnwrapKey,
	.C_DeriveKey = C_DeriveKey,

	// Random
	.C_SeedRandom = C_SeedRandom,
	.C_GenerateRandom = C_GenerateRandom,

	// Parallel
	.C_GetFunctionStatus = C_GetFunctionStatus,
	.C_CancelFunction = C_CancelFunction,
	.C_WaitForSlotEvent = C_WaitForSlotEvent};

CK_RV C_GetFunctionList(CK_FUNCTION_LIST_PTR_PTR ppFunctionList) {
	if (ppFunctionList == NULL_PTR) {
		return CKR_ARGUMENTS_BAD;
	}

	*ppFunctionList = &g_function_list_3_0;
	return CKR_OK;
}
