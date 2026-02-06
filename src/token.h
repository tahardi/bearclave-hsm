#ifndef BEARCLAVE_TOKEN_H
#define BEARCLAVE_TOKEN_H

// NOLINTBEGIN
#define TOKEN_LABEL_LEN 32
#define TOKEN_MAX_SESSIONS 8
#define TOKEN_MAX_SESSIONS_RW 8
#define TOKEN_MAX_PIN_LEN 32
#define TOKEN_MIN_PIN_LEN 4
#define TOKEN_MODEL_LEN 16
#define TOKEN_SERIAL_LEN 16
#define TOKEN_TIME_LEN 16
#define TOKEN_TOTAL_PUBLIC_MEMORY 1024
#define TOKEN_TOTAL_PRIVATE_MEMORY 1024
// NOLINTEND

#include "common.h"
#include "mech.h"

typedef struct token token_t;

typedef struct {
	unsigned char label[TOKEN_LABEL_LEN];
	unsigned char man_id[MAN_ID_LEN];
	unsigned char model[TOKEN_MODEL_LEN];
	unsigned char serial[TOKEN_SERIAL_LEN];
	unsigned char utc_time[TOKEN_TIME_LEN];

	unsigned long flags;
	unsigned long ulMaxSessionCount;
	unsigned long ulSessionCount;
	unsigned long ulMaxRwSessionCount;
	unsigned long ulRwSessionCount;
	unsigned long ulMaxPinLen;
	unsigned long ulMinPinLen;
	unsigned long ulTotalPublicMemory;
	unsigned long ulFreePublicMemory;
	unsigned long ulTotalPrivateMemory;
	unsigned long ulFreePrivateMemory;

	version_t fw_version;
	version_t hw_version;
} token_info_t;

typedef enum {
	TOKEN_OK = 0,
	TOKEN_ERR_BAD_ARGS,
	TOKEN_ERR_MECH,
	TOKEN_ERR_MECH_NOT_FOUND,
	TOKEN_ERR_MECH_ALREADY_EXISTS,
	TOKEN_ERR_MEMORY,
	TOKEN_ERR_NOT_INITIALIZED,
	TOKEN_ERR_SAFE
} token_error_t;

token_t *token_new(unsigned char man_id[MAN_ID_LEN],
		   unsigned char model[TOKEN_MODEL_LEN],
		   unsigned char serial[TOKEN_SERIAL_LEN], version_t fw_version,
		   version_t hw_version);
void token_free(token_t *token);

token_error_t token_initialize(token_t *token, unsigned char label[TOKEN_LABEL_LEN],
			 unsigned char *so_pin, unsigned long so_pin_len);
token_error_t token_get_info(token_t *token, token_info_t *info);
token_error_t token_add_mech(token_t *token, mech_t *mech);
token_error_t token_get_mech(token_t *token, unsigned long mech_type,
			     mech_t **mech);
token_error_t token_get_mechs_len(token_t *token, unsigned long *mechs_len);
token_error_t token_get_mech_list(token_t *token, unsigned long *mech_list,
				  unsigned long mechs_len);
token_error_t token_get_so_pin(token_t *token, unsigned char **so_pin,
			      unsigned long *so_pin_len);

#endif // BEARCLAVE_TOKEN_H
