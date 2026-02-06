#include "slot.h"

#include "safe.h"
#include "token.h"

#include <stdlib.h>
#include <string.h>

typedef struct slot {
	unsigned long slot_id;
	slot_info_t *info;
	token_t *token;
} slot_t;

slot_t *slot_new(unsigned long slot_id, unsigned char desc[SLOT_DESC_SIZE],
		 unsigned char man_id[MAN_ID_SIZE]) {
	slot_t *slot = malloc(sizeof(slot_t));
	if (slot == NULL) {
		return NULL;
	}

	slot->info = malloc(sizeof(slot_info_t));
	if (slot->info == NULL) {
		free(slot);
		return NULL;
	}

	slot->slot_id = slot_id;
	slot->token = NULL;
	slot->info->flags = 0;
	slot->info->fw_version.major = FW_VERSION_MAJOR;
	slot->info->fw_version.minor = FW_VERSION_MINOR;
	slot->info->hw_version.major = HW_VERSION_MAJOR;
	slot->info->hw_version.minor = HW_VERSION_MINOR;
	safe_memcpy(slot->info->slot_desc, SLOT_DESC_SIZE, desc,
		    SLOT_DESC_SIZE);
	safe_memcpy(slot->info->man_id, MAN_ID_SIZE, man_id, MAN_ID_SIZE);
	return slot;
}

void slot_free(slot_t *slot) {
	if (slot == NULL) {
		return;
	}
	free(slot);
}

bool slot_has_token(slot_t *slot) {
	return slot->token != NULL;
}

slot_error_t slot_get_info(slot_t *slot, slot_info_t *info) {
	if (slot == NULL || info == NULL) {
		return SLOT_ERR_BAD_ARGS;
	}

	info->flags = slot->info->flags;
	info->fw_version = slot->info->fw_version;
	info->hw_version = slot->info->hw_version;

	int err = safe_memcpy(info->man_id, MAN_ID_SIZE, slot->info->man_id,
			      MAN_ID_SIZE);
	if (err != SAFE_OK) {
		return err;
	}
	err = safe_memcpy(info->slot_desc, SLOT_DESC_SIZE,
			  slot->info->slot_desc, SLOT_DESC_SIZE);
	if (err != SAFE_OK) {
		return err;
	}
	return SLOT_OK;
}

slot_error_t slot_get_id(slot_t *slot, unsigned long *slot_id) {
	if (slot == NULL || slot_id == NULL) {
		return SLOT_ERR_BAD_ARGS;
	}
	*slot_id = slot->slot_id;
	return SLOT_OK;
}
