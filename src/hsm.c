#include "hsm.h"
#include "safe.h"
#include "slot.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct hsm {
	bool initialized;

	hsm_info_t *info;
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

	g_hsm.info = malloc(sizeof(hsm_info_t));
	if (g_hsm.info == NULL) {
		return HSM_ERR_MEMORY;
	}

	g_hsm.info->flags = FLAGS;
	g_hsm.info->ck_version.major = CK_VERSION_MAJOR;
	g_hsm.info->ck_version.minor = CK_VERSION_MINOR;
	g_hsm.info->lib_version.major = LIB_VERSION_MAJOR;
	g_hsm.info->lib_version.minor = LIB_VERSION_MINOR;

	int err = safe_memcpy_with_padding(g_hsm.info->lib_desc,
					   sizeof(g_hsm.info->lib_desc),
					   LIB_DESC, strlen(LIB_DESC), PAD_VAL);
	if (err != SAFE_OK) {
		free(g_hsm.info);
		return HSM_ERR_SAFE;
	}

	err = safe_memcpy_with_padding(g_hsm.info->man_id,
				       sizeof(g_hsm.info->man_id), MAN_ID,
				       strlen(MAN_ID), PAD_VAL);
	if (err != SAFE_OK) {
		free(g_hsm.info);
		return HSM_ERR_SAFE;
	}
	return HSM_OK;
}

hsm_error_t hsm_finalize(void) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	g_hsm.initialized = false;
	return HSM_OK;
}

bool hsm_is_initialized(void) {
	return g_hsm.initialized;
}

hsm_error_t hsm_get_info(hsm_info_t *info) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	if (info == NULL) {
		return HSM_ERR_BAD_ARGS;
	}

	info->ck_version.major = g_hsm.info->ck_version.major;
	info->ck_version.minor = g_hsm.info->ck_version.minor;
	info->lib_version.major = g_hsm.info->lib_version.major;
	info->lib_version.minor = g_hsm.info->lib_version.minor;
	info->flags = g_hsm.info->flags;

	int err =
		safe_memcpy(info->lib_desc, sizeof(info->lib_desc),
			    g_hsm.info->lib_desc, sizeof(g_hsm.info->lib_desc));
	if (err != SAFE_OK) {
		return HSM_ERR_SAFE;
	}

	err = safe_memcpy(info->man_id, sizeof(info->man_id),
			  g_hsm.info->man_id, sizeof(g_hsm.info->man_id));
	if (err != SAFE_OK) {
		return HSM_ERR_SAFE;
	}
	return HSM_OK;
}

hsm_error_t hsm_add_slot(slot_t *slot) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	if (slot == NULL) {
		return HSM_ERR_BAD_ARGS;
	}

	unsigned long slot_id = 0;
	int err = slot_get_id(slot, &slot_id);
	if (err != SLOT_OK) {
		return HSM_ERR_SLOT;
	}

	slot_t *existing_slot;
	err = hsm_get_slot(slot_id, &existing_slot);
	if (err != HSM_ERR_SLOT_NOT_FOUND) {
		return HSM_ERR_SLOT_ALREADY_EXISTS;
	}

	g_hsm.slots_len++;
	slot_t **new_slots = (slot_t **)realloc(
		(void *)g_hsm.slots, g_hsm.slots_len * sizeof(slot_t *));
	if (new_slots == NULL) {
		g_hsm.slots_len--;
		return HSM_ERR_MEMORY;
	}
	g_hsm.slots = new_slots;
	g_hsm.slots[g_hsm.slots_len - 1] = slot;
	return HSM_OK;
}

hsm_error_t hsm_get_slot(unsigned long slot_id, slot_t **slot) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	if (slot == NULL) {
		return HSM_ERR_BAD_ARGS;
	}

	for (size_t i = 0; i < g_hsm.slots_len; i++) {
		unsigned long sid;
		int err = slot_get_id(g_hsm.slots[i], &sid);
		if (err != SLOT_OK) {
			continue;
		}
		if (slot_id == sid) {
			*slot = g_hsm.slots[i];
			return HSM_OK;
		}
	}
	return HSM_ERR_SLOT_NOT_FOUND;
}

hsm_error_t hsm_get_slots_len(bool tokenPresent, unsigned long *slots_len) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	if (slots_len == NULL) {
		return HSM_ERR_BAD_ARGS;
	}

	unsigned long len = 0;
	for (size_t i = 0; i < g_hsm.slots_len; i++) {
		if (slot_has_token(g_hsm.slots[i]) && tokenPresent) {
			len++;
		}
	}
	*slots_len = len;
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
				return HSM_ERR_SLOT;
			}
			slot_list[i] = slot_id;
		}
	}
	return HSM_OK;
}

hsm_error_t hsm_get_slot_info(unsigned long slot_id, slot_info_t *info) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	if (info == NULL) {
		return HSM_ERR_BAD_ARGS;
	}

	for (size_t i = 0; i < g_hsm.slots_len; i++) {
		slot_t *slot = g_hsm.slots[i];
		unsigned long sid;
		int err = slot_get_id(slot, &sid);
		if (err != SLOT_OK) {
			return HSM_ERR_SLOT;
		}
		if (slot_id != sid) {
			continue;
		}

		err = slot_get_info(slot, info);
		if (err != SLOT_OK) {
			return HSM_ERR_SLOT;
		}
	}
	return HSM_OK;
}
