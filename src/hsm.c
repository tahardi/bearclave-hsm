#include "hsm.h"
#include "safe.h"
#include "slot.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct hsm {
	bool initialized;

	slot_t **slots;
	uint8_t slots_len;
} hsm_t;

hsm_t g_hsm = {.initialized = false};

hsm_error_t hsm_initialize(void) {
	if (g_hsm.initialized) {
		return HSM_OK;
	}

	g_hsm.initialized = true;
	g_hsm.slots = NULL;
	g_hsm.slots_len = 0;
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

	unsigned char *dst = info->lib_desc;
	unsigned char *src = (unsigned char *)LIB_DESC;
	size_t dst_len = sizeof(info->lib_desc);
	size_t src_len = strlen(LIB_DESC);
	int err = safe_memcpy_with_padding(dst, dst_len, src, src_len, PAD_VAL);
	if (err != SAFE_OK) {
		return err;
	}

	dst = info->man_id;
	src = (unsigned char *)MAN_ID;
	dst_len = sizeof(info->man_id);
	src_len = strlen(MAN_ID);
	err = safe_memcpy_with_padding(dst, dst_len, src, src_len, PAD_VAL);
	if (err != SAFE_OK) {
		return err;
	}
	return HSM_OK;
}

hsm_error_t hsm_get_slot_len(bool tokenPresent, unsigned long *slot_count) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	if (slot_count == NULL) {
		return HSM_ERR_BAD_ARGS;
	}

	unsigned long slots_len = 0;
	for (size_t i = 0; i < g_hsm.slots_len; i++) {
		if (slot_has_token(g_hsm.slots[i]) && tokenPresent) {
			slots_len++;
		}
	}
	*slot_count = slots_len;
	return HSM_OK;
}

hsm_error_t hsm_get_slot_list(bool tokenPresent, unsigned long *slot_list,
			      unsigned long slot_count) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	if (slot_list == NULL || slot_count > g_hsm.slots_len) {
		return HSM_ERR_BAD_ARGS;
	}

	for (size_t i = 0; i < g_hsm.slots_len; i++) {
		if (slot_has_token(g_hsm.slots[i]) && tokenPresent) {
			unsigned long slot_id;
			int err = slot_get_id(g_hsm.slots[i], &slot_id);
			if (err != SLOT_OK) {
				return err;
			}
			slot_list[i] = slot_id;
		}
	}
	return HSM_OK;
}
