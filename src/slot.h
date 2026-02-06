#ifndef BEARCLAVE_SLOT_H
#define BEARCLAVE_SLOT_H
#include "common.h"
#include "token.h"

#include <stdbool.h>

typedef struct slot slot_t;

typedef struct {
	unsigned char man_id[MAN_ID_SIZE];
	unsigned char slot_desc[SLOT_DESC_SIZE];
	unsigned long flags;
	version_t fw_version;
	version_t hw_version;
} slot_info_t;

typedef enum {
	SLOT_OK = 0,
	SLOT_ERR_BAD_ARGS,
} slot_error_t;

slot_t *slot_new(unsigned long slot_id, unsigned char desc[SLOT_DESC_SIZE],
		 unsigned char man_id[MAN_ID_SIZE]);
void slot_free(slot_t *slot);

bool slot_has_token(slot_t *slot);
slot_error_t slot_get_id(slot_t *slot, unsigned long *slot_id);
slot_error_t slot_get_info(slot_t *slot, slot_info_t *info);
slot_error_t slot_get_token(slot_t *slot, token_t **token);

#endif // BEARCLAVE_SLOT_H
