// NOLINTBEGIN(readability-non-const-parameter,bugprone-easily-swappable-parameters)
#include "bearclave/pkcs11.h"
#include "hsm.h"
#include "safe.h"

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

	hsm_info_t hsm_info;
	int err = hsm_get_info(&hsm_info);
	if (err != HSM_OK) {
		return CKR_DEVICE_ERROR;
	}
	pInfo->cryptokiVersion.major = hsm_info.ck_version.major;
	pInfo->cryptokiVersion.minor = hsm_info.ck_version.minor;
	pInfo->libraryVersion.major = hsm_info.lib_version.major;
	pInfo->libraryVersion.minor = hsm_info.lib_version.minor;
	pInfo->flags = hsm_info.flags;

	err = safe_memcpy(pInfo->manufacturerID, sizeof(pInfo->manufacturerID),
			  hsm_info.man_id, sizeof(hsm_info.man_id));
	if (err != SAFE_OK) {
		return CKR_DEVICE_ERROR;
	}

	err = safe_memcpy(pInfo->libraryDescription,
			  sizeof(pInfo->libraryDescription), hsm_info.lib_desc,
			  sizeof(hsm_info.lib_desc));
	if (err != SAFE_OK) {
		return CKR_DEVICE_ERROR;
	}
	return CKR_OK;
}

// NOLINTEND(readability-non-const-parameter,bugprone-easily-swappable-parameters)
