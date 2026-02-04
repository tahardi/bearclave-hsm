#include "hsm.h"
#include "safe.h"
#include "slot.h"

#include <stdbool.h>
#include <string.h>

typedef struct hsm {
	bool initialized;

	slot_t *slots;
} hsm_t;

hsm_t g_hsm = {.initialized = false};

hsm_error_t hsm_initialize(void) {
	if (g_hsm.initialized) {
		return HSM_OK;
	}
	g_hsm.initialized = true;
	return HSM_OK;
}

hsm_error_t hsm_finalize(void) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	g_hsm.initialized = false;
	return HSM_OK;
}

hsm_error_t hsm_get_info(hsm_info_t *info) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	if (info == NULL) {
		return HSM_ERR_BAD_ARGS;
	}

	info->ck_version.major = CK_VERSION_MAJOR;
	info->ck_version.minor = CK_VERSION_MINOR;
	info->lib_version.major = LIB_VERSION_MAJOR;
	info->lib_version.minor = LIB_VERSION_MINOR;
	info->flags = FLAGS;

	// Note that pkcs11t.h states the strings must be blank space padded.
	const size_t desc_len = strlen(LIB_DESC);
	const size_t desc_pad_len = sizeof(info->lib_desc) - desc_len;
	int ret = safe_memcpy(info->lib_desc, sizeof(info->lib_desc), LIB_DESC,
			      desc_len);
	if (ret != SAFE_OK) {
		return HSM_ERR_MEMCPY;
	}
	ret = safe_memset(info->lib_desc + desc_len, desc_pad_len, desc_pad_len,
			  PAD_VAL);
	if (ret != SAFE_OK) {
		return HSM_ERR_MEMSET;
	}

	const size_t id_len = strlen(MAN_ID);
	const size_t id_pad_len = sizeof(info->man_id) - id_len;
	ret = safe_memcpy(info->man_id, sizeof(info->man_id), MAN_ID, id_len);
	if (ret != SAFE_OK) {
		return ret;
	}
	ret = safe_memset(info->man_id + id_len, id_pad_len, id_pad_len,
			  PAD_VAL);
	if (ret != SAFE_OK) {
		return ret;
	}
	return HSM_OK;
}
