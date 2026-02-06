// NOLINTBEGIN(readability-non-const-parameter,bugprone-easily-swappable-parameters)
#include "bearclave/pkcs11.h"
#include "hsm.h"
#include "safe.h"

#include <stdlib.h>

// This is a two-stage function. The caller makes an initial call to
// populate the slot count variable. They should then allocate memory
// for pSlotList and call again so that we can populate pSlotList.
CK_RV C_GetSlotList(CK_BBOOL tokenPresent, CK_SLOT_ID_PTR pSlotList,
		    CK_ULONG_PTR pulCount) {
	if (pulCount == NULL) {
		return CKR_ARGUMENTS_BAD;
	}

	bool tokPres = tokenPresent == CK_TRUE;
	unsigned long slots_len = 0;
	int err = hsm_get_slots_len(tokPres, &slots_len);
	if (err != HSM_OK) {
		return CKR_GENERAL_ERROR;
	}

	if (pSlotList == NULL) {
		*pulCount = slots_len;
		return CKR_OK;
	}

	unsigned long *slot_ids = malloc(slots_len * sizeof(unsigned long));
	if (slot_ids == NULL) {
		return CKR_HOST_MEMORY;
	}

	err = hsm_get_slot_list(tokPres, slot_ids, slots_len);
	if (err != HSM_OK) {
		free(slot_ids);
		return CKR_GENERAL_ERROR;
	}

	size_t slot_ids_size = slots_len * sizeof(unsigned long);
	err = safe_memcpy(pSlotList, slot_ids_size, slot_ids, slot_ids_size);
	if (err != SAFE_OK) {
		free(slot_ids);
		return CKR_GENERAL_ERROR;
	}
	free(slot_ids);
	return CKR_OK;
}

CK_RV C_GetSlotInfo(CK_SLOT_ID slotID, CK_SLOT_INFO_PTR pInfo) {
	if (pInfo == NULL) {
		return CKR_ARGUMENTS_BAD;
	}

	slot_t *slot;
	int err = hsm_get_slot(slotID, &slot);
	if (err != HSM_OK) {
		return CKR_GENERAL_ERROR;
	}

	slot_info_t info;
	err = slot_get_info(slot, &info);
	if (err != SLOT_OK) {
		return CKR_GENERAL_ERROR;
	}

	pInfo->flags = info.flags;
	pInfo->hardwareVersion.major = info.hw_version.major;
	pInfo->hardwareVersion.minor = info.hw_version.minor;
	pInfo->firmwareVersion.major = info.fw_version.major;
	pInfo->firmwareVersion.minor = info.fw_version.minor;

	err = safe_memcpy(pInfo->manufacturerID, sizeof(pInfo->manufacturerID),
			  info.man_id, sizeof(info.man_id));
	if (err != SAFE_OK) {
		return CKR_GENERAL_ERROR;
	}
	err = safe_memcpy(pInfo->slotDescription,
			  sizeof(pInfo->slotDescription), info.slot_desc,
			  sizeof(info.slot_desc));
	if (err != SAFE_OK) {
		return CKR_GENERAL_ERROR;
	}
	return CKR_OK;
}

CK_RV C_GetTokenInfo(CK_SLOT_ID slotID, CK_TOKEN_INFO_PTR pInfo) {
	if (pInfo == NULL) {
		return CKR_ARGUMENTS_BAD;
	}

	slot_t *slot;
	int err = hsm_get_slot(slotID, &slot);
	if (err != HSM_OK) {
		return CKR_GENERAL_ERROR;
	}

	token_t *token;
	err = slot_get_token(slot, &token);
	if (err != SLOT_OK) {
		return CKR_GENERAL_ERROR;
	}

	token_info_t info;
	err = token_get_info(token, &info);
	if (err != TOKEN_OK) {
		return CKR_GENERAL_ERROR;
	}

	pInfo->flags = info.flags;
	pInfo->ulMaxSessionCount = info.ulMaxSessionCount;
	pInfo->ulSessionCount = info.ulSessionCount;
	pInfo->ulMaxRwSessionCount = info.ulMaxRwSessionCount;
	pInfo->ulRwSessionCount = info.ulRwSessionCount;
	pInfo->ulMaxPinLen = info.ulMaxPinLen;
	pInfo->ulMinPinLen = info.ulMinPinLen;
	pInfo->ulTotalPublicMemory = info.ulTotalPublicMemory;
	pInfo->ulFreePublicMemory = info.ulFreePublicMemory;
	pInfo->ulTotalPrivateMemory = info.ulTotalPrivateMemory;
	pInfo->ulFreePrivateMemory = info.ulFreePrivateMemory;
	pInfo->firmwareVersion.major = info.fw_version.major;
	pInfo->firmwareVersion.minor = info.fw_version.minor;
	pInfo->hardwareVersion.major = info.hw_version.major;
	pInfo->hardwareVersion.minor = info.hw_version.minor;

	err = safe_memcpy(pInfo->label, sizeof(pInfo->label), info.label,
			  sizeof(info.label));
	if (err != SAFE_OK) {
		return CKR_GENERAL_ERROR;
	}

	err = safe_memcpy(pInfo->manufacturerID, sizeof(pInfo->manufacturerID),
			  info.man_id, sizeof(info.man_id));
	if (err != SAFE_OK) {
		return CKR_GENERAL_ERROR;
	}

	err = safe_memcpy(pInfo->model, sizeof(pInfo->model), info.model,
			  sizeof(info.model));
	if (err != SAFE_OK) {
		return CKR_GENERAL_ERROR;
	}

	err = safe_memcpy(pInfo->serialNumber, sizeof(pInfo->serialNumber),
			  info.serial, sizeof(info.serial));
	if (err != SAFE_OK) {
		return CKR_GENERAL_ERROR;
	}

	err = safe_memcpy(pInfo->utcTime, sizeof(pInfo->utcTime), info.utc_time,
			  sizeof(info.utc_time));
	if (err != SAFE_OK) {
		return CKR_GENERAL_ERROR;
	}
	return CKR_OK;
}

CK_RV C_GetMechanismList(CK_SLOT_ID slotID,
			 CK_MECHANISM_TYPE_PTR pMechanismList,
			 CK_ULONG_PTR pulCount) {
	if (pulCount == NULL) {
		return CKR_ARGUMENTS_BAD;
	}

	slot_t *slot;
	int err = hsm_get_slot(slotID, &slot);
	if (err != HSM_OK) {
		return CKR_GENERAL_ERROR;
	}

	token_t *token;
	err = slot_get_token(slot, &token);
	if (err != SLOT_OK) {
		return CKR_GENERAL_ERROR;
	}

	unsigned long mechs_len = 0;
	err = token_get_mechs_len(token, &mechs_len);
	if (err != TOKEN_OK) {
		return CKR_GENERAL_ERROR;
	}

	if (pMechanismList == NULL) {
		*pulCount = mechs_len;
		return CKR_OK;
	}

	unsigned long *mech_types = malloc(mechs_len * sizeof(unsigned long));
	if (mech_types == NULL) {
		return CKR_HOST_MEMORY;
	}

	err = token_get_mech_list(token, mech_types, mechs_len);
	if (err != TOKEN_OK) {
		free(mech_types);
		return CKR_GENERAL_ERROR;
	}

	size_t mech_types_size = mechs_len * sizeof(unsigned long);
	err = safe_memcpy(pMechanismList, mech_types_size, mech_types,
			  mech_types_size);
	if (err != SAFE_OK) {
		free(mech_types);
		return CKR_GENERAL_ERROR;
	}
	free(mech_types);
	return CKR_OK;
}

CK_RV C_GetMechanismInfo(CK_SLOT_ID slotID, CK_MECHANISM_TYPE type,
			 CK_MECHANISM_INFO_PTR pInfo) {
	if (pInfo == NULL) {
		return CKR_ARGUMENTS_BAD;
	}

	slot_t *slot;
	int err = hsm_get_slot(slotID, &slot);
	if (err != HSM_OK) {
		return CKR_GENERAL_ERROR;
	}

	token_t *token;
	err = slot_get_token(slot, &token);
	if (err != SLOT_OK) {
		return CKR_GENERAL_ERROR;
	}

	mech_t *mech;
	err = token_get_mech(token, type, &mech);
	if (err != TOKEN_OK) {
		return CKR_GENERAL_ERROR;
	}

	mech_info_t info;
	err = mech_get_info(mech, &info);
	if (err != MECH_OK) {
		return CKR_GENERAL_ERROR;
	}

	return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV C_InitToken(CK_SLOT_ID slotID, CK_UTF8CHAR_PTR pPin, CK_ULONG ulPinLen,
		  CK_UTF8CHAR_PTR pLabel) {
	if (pPin == NULL || pLabel == NULL) {
		return CKR_ARGUMENTS_BAD;
	}

	slot_t *slot;
	int err = hsm_get_slot(slotID, &slot);
	if (err != HSM_OK) {
		return CKR_GENERAL_ERROR;
	}

	token_t *token;
	err = slot_get_token(slot, &token);
	if (err != SLOT_OK) {
		return CKR_GENERAL_ERROR;
	}

	err = token_initialize(token, pLabel, pPin, ulPinLen);
	if (err != TOKEN_OK) {
		return CKR_GENERAL_ERROR;
	}
	return CKR_OK;
}

CK_RV C_InitPIN(CK_SESSION_HANDLE hSession, CK_UTF8CHAR_PTR pPin,
		CK_ULONG ulPinLen) {
	(void)hSession;
	(void)pPin;
	(void)ulPinLen;
	return CKR_FUNCTION_NOT_SUPPORTED;
}

CK_RV C_SetPIN(CK_SESSION_HANDLE hSession, CK_UTF8CHAR_PTR pOldPin,
	       CK_ULONG ulOldLen, CK_UTF8CHAR_PTR pNewPin, CK_ULONG ulNewLen) {
	(void)hSession;
	(void)pOldPin;
	(void)ulOldLen;
	(void)pNewPin;
	(void)ulNewLen;
	return CKR_FUNCTION_NOT_SUPPORTED;
}
// NOLINTEND(readability-non-const-parameter,bugprone-easily-swappable-parameters)
