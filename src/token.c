#include "token.h"

#include "hsm.h"
#include "safe.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct token {
	bool initialized;

	token_info_t *info;
	mech_t **mechs;
	uint8_t mechs_len;

	unsigned char *so_pin;
	unsigned long so_pin_len;
} token_t;

token_t *token_new(unsigned char man_id[MAN_ID_SIZE],
		   unsigned char model[MODEL_SIZE],
		   unsigned char serial[SERIAL_SIZE], version_t fw_version,
		   version_t hw_version) {
	if (man_id == NULL || model == NULL || serial == NULL) {
		return NULL;
	}

	token_t *token = malloc(sizeof(token_t));
	if (token == NULL) {
		return NULL;
	}

	token->info = malloc(sizeof(token_info_t));
	if (token->info == NULL) {
		free(token);
		return NULL;
	}

	token->initialized = false;
	token->info->flags = 0;
	token->info->ulMaxSessionCount = TOKEN_MAX_SESSIONS;
	token->info->ulSessionCount = 0;
	token->info->ulMaxRwSessionCount = TOKEN_MAX_SESSIONS_RW;
	token->info->ulMaxPinLen = TOKEN_MAX_PIN_LEN;
	token->info->ulMinPinLen = TOKEN_MIN_PIN_LEN;
	token->info->ulTotalPublicMemory = TOKEN_TOTAL_PUBLIC_MEMORY;
	token->info->ulFreePublicMemory = TOKEN_TOTAL_PUBLIC_MEMORY;
	token->info->ulTotalPrivateMemory = TOKEN_TOTAL_PRIVATE_MEMORY;
	token->info->ulFreePrivateMemory = TOKEN_TOTAL_PRIVATE_MEMORY;
	token->info->fw_version = fw_version;
	token->info->hw_version = hw_version;

	int err = safe_memcpy(token->info->man_id, MAN_ID_SIZE, man_id,
			      MAN_ID_SIZE);
	if (err != SAFE_OK) {
		free(token);
		return NULL;
	}

	err = safe_memcpy(token->info->model, MODEL_SIZE, model, MODEL_SIZE);
	if (err != SAFE_OK) {
		free(token);
		return NULL;
	}

	err = safe_memcpy(token->info->serial, SERIAL_SIZE, serial,
			  SERIAL_SIZE);
	if (err != SAFE_OK) {
		free(token);
		return NULL;
	}
	return token;
}

void token_free(token_t *token) {
	if (token == NULL) {
		return;
	}

	if (token->so_pin != NULL) {
		free(token->so_pin);
	}

	for (size_t i = 0; i < token->mechs_len; i++) {
		mech_free(token->mechs[i]);
	}
	if (token->mechs != NULL) {
		free((void *)token->mechs);
	}

	free(token);
}

token_error_t token_init(token_t *token, unsigned char label[LABEL_SIZE],
			 unsigned char *pin, unsigned long pin_len) {
	if (token == NULL || pin == NULL) {
		return TOKEN_ERR_BAD_ARGS;
	}

	int err = safe_memcpy(label, LABEL_SIZE, label, LABEL_SIZE);
	if (err != SAFE_OK) {
		return TOKEN_ERR_SAFE;
	}

	if (token->so_pin != NULL) {
		free(token->so_pin);
	}

	token->so_pin = malloc(pin_len);
	err = safe_memcpy(token->so_pin, pin_len, pin, pin_len);
	if (err != SAFE_OK) {
		return TOKEN_ERR_SAFE;
	}
	token->so_pin_len = pin_len;
	token->initialized = true;
	return TOKEN_OK;
}

token_error_t token_get_info(token_t *token, token_info_t *info) {
	if (token == NULL || info == NULL) {
		return TOKEN_ERR_BAD_ARGS;
	}
	if (!token->initialized) {
		return TOKEN_ERR_NOT_INITIALIZED;
	}

	info->flags = token->info->flags;
	info->ulMaxSessionCount = token->info->ulMaxSessionCount;
	info->ulSessionCount = token->info->ulSessionCount;
	info->ulMaxRwSessionCount = token->info->ulMaxRwSessionCount;
	info->ulRwSessionCount = token->info->ulRwSessionCount;
	info->ulMaxPinLen = token->info->ulMaxPinLen;
	info->ulMinPinLen = token->info->ulMinPinLen;
	info->ulTotalPublicMemory = token->info->ulTotalPublicMemory;
	info->ulFreePublicMemory = token->info->ulFreePublicMemory;
	info->ulTotalPrivateMemory = token->info->ulTotalPrivateMemory;
	info->ulFreePrivateMemory = token->info->ulFreePrivateMemory;
	info->fw_version = token->info->fw_version;
	info->hw_version = token->info->hw_version;

	int err = safe_memcpy(info->label, LABEL_SIZE, token->info->label,
			      LABEL_SIZE);
	if (err != SAFE_OK) {
		return TOKEN_ERR_SAFE;
	}

	err = safe_memcpy(info->man_id, MAN_ID_SIZE, token->info->man_id,
			  MAN_ID_SIZE);
	if (err != SAFE_OK) {
		return TOKEN_ERR_SAFE;
	}

	err = safe_memcpy(info->model, MODEL_SIZE, token->info->model,
			  MODEL_SIZE);
	if (err != SAFE_OK) {
		return TOKEN_ERR_SAFE;
	}

	err = safe_memcpy(info->serial, SERIAL_SIZE, token->info->serial,
			  SERIAL_SIZE);
	if (err != SAFE_OK) {
		return TOKEN_ERR_SAFE;
	}

	err = safe_memcpy(info->utc_time, TIME_SIZE, token->info->utc_time,
			  TIME_SIZE);
	if (err != SAFE_OK) {
		return TOKEN_ERR_SAFE;
	}
	return TOKEN_OK;
}

token_error_t token_get_mech(token_t *token, unsigned long mech_type,
			     mech_t **mech) {
	if (token == NULL || mech == NULL) {
		return TOKEN_ERR_BAD_ARGS;
	}
	if (!token->initialized) {
		return TOKEN_ERR_NOT_INITIALIZED;
	}

	for (size_t i = 0; i < token->mechs_len; i++) {
		mech_info_t info;
		int err = mech_get_info(token->mechs[i], &info);
		if (err != MECH_OK) {
			return TOKEN_ERR_MECH;
		}
		if (info.type == mech_type) {
			*mech = token->mechs[i];
			return TOKEN_OK;
		}
	}
	return TOKEN_ERR_MECH_NOT_FOUND;
}

token_error_t token_get_mechs_len(token_t *token, unsigned long *mechs_len) {
	if (token == NULL || mechs_len == NULL) {
		return TOKEN_ERR_BAD_ARGS;
	}
	if (!token->initialized) {
		return TOKEN_ERR_NOT_INITIALIZED;
	}
	*mechs_len = token->mechs_len;
	return TOKEN_OK;
}

token_error_t token_get_mech_list(token_t *token, unsigned long *mech_list,
				  unsigned long mechs_len) {
	if (token == NULL || mech_list == NULL) {
		return TOKEN_ERR_BAD_ARGS;
	}
	if (!token->initialized) {
		return TOKEN_ERR_NOT_INITIALIZED;
	}
	if (mechs_len != token->mechs_len) {
		return TOKEN_ERR_BAD_ARGS;
	}

	for (size_t i = 0; i < token->mechs_len; i++) {
		mech_info_t info;
		int err = mech_get_info(token->mechs[i], &info);
		if (err != MECH_OK) {
			return TOKEN_ERR_MECH;
		}
		mech_list[i] = info.type;
	}
	return TOKEN_OK;
}
