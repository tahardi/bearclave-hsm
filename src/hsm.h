#ifndef BEARCLAVE_HSM_H
#define BEARCLAVE_HSM_H

#include "common.h"
#include "slot.h"

#include <stdbool.h>

typedef struct hsm hsm_t;

typedef struct {
	unsigned char man_id[MAN_ID_SIZE];
	unsigned char lib_desc[LIB_DESC_SIZE];
	unsigned long flags;
	version_t ck_version;
	version_t lib_version;
} hsm_info_t;

typedef enum {
	HSM_OK = 0,
	HSM_ERR_NOT_INITIALIZED,
	HSM_ERR_BAD_ARGS,
	HSM_ERR_MEMORY,
	HSM_ERR_SAFE,
	HSM_ERR_SLOT,
	HSM_ERR_SLOT_NOT_FOUND,
	HSM_ERR_SLOT_ALREADY_EXISTS
} hsm_error_t;

extern hsm_t g_hsm;

hsm_error_t hsm_initialize(void);
hsm_error_t hsm_finalize(void);
bool hsm_is_initialized(void);
hsm_error_t hsm_get_info(hsm_info_t *info);
hsm_error_t hsm_add_slot(slot_t *slot);
hsm_error_t hsm_get_slot(unsigned long slot_id, slot_t **slot);
hsm_error_t hsm_get_slots_len(bool tokenPresent, unsigned long *slots_len);
hsm_error_t hsm_get_slot_list(bool tokenPresent, unsigned long *slot_list,
			      unsigned long slot_count);

#endif // BEARCLAVE_HSM_H
