// NOLINTBEGIN(readability-non-const-parameter,bugprone-easily-swappable-parameters)
#include "bearclave/pkcs11.h"
#include "hsm.h"

CK_RV C_Initialize(CK_VOID_PTR pInitArgs) {
	(void)pInitArgs;
	if (hsm_initialize() != 0) {
		return CKR_DEVICE_ERROR;
	}
	return CKR_OK;
}

CK_RV C_Finalize(CK_VOID_PTR pReserved) {
	(void)pReserved;
	if (hsm_finalize() != 0) {
		return CKR_DEVICE_ERROR;
	}
	return CKR_OK;
}

CK_RV C_GetInfo(CK_INFO_PTR pInfo) {
	if (pInfo == NULL_PTR) {
		return CKR_ARGUMENTS_BAD;
	}

	int ret = hsm_get_ck_version_major(&pInfo->cryptokiVersion.major);
	if (ret != HSM_OK) {
		return CKR_DEVICE_ERROR;
	}
	ret = hsm_get_ck_version_minor(&pInfo->cryptokiVersion.minor);
	if (ret != HSM_OK) {
		return CKR_DEVICE_ERROR;
	}

	ret = hsm_get_flags(&pInfo->flags);
	if (ret != HSM_OK) {
		return CKR_DEVICE_ERROR;
	}

	ret = hsm_get_lib_version_major(&pInfo->libraryVersion.major);
	if (ret != HSM_OK) {
		return CKR_DEVICE_ERROR;
	}
	ret = hsm_get_lib_version_minor(&pInfo->libraryVersion.minor);
	if (ret != HSM_OK) {
		return CKR_DEVICE_ERROR;
	}

	ret = hsm_get_lib_desc(pInfo->libraryDescription,
			       LIBRARY_DESCRIPTION_SIZE);
	if (ret != HSM_OK) {
		return CKR_DEVICE_ERROR;
	}

	ret = hsm_get_man_id(pInfo->manufacturerID, MANUFACTURER_ID_SIZE);
	if (ret != HSM_OK) {
		return CKR_DEVICE_ERROR;
	}
	return CKR_OK;
}

// NOLINTEND(readability-non-const-parameter,bugprone-easily-swappable-parameters)
