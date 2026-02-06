#ifndef BEARCLAVE_TOKEN_H
#define BEARCLAVE_TOKEN_H
#include "common.h"
#include "mechanism.h"

typedef struct token token_t;

typedef struct {
	unsigned char label[LABEL_SIZE];
	unsigned char man_id[MAN_ID_SIZE];
	unsigned char model[MODEL_SIZE];
	unsigned char serial[SERIAL_SIZE];
	unsigned char utc_time[TIME_SIZE];

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
	TOKEN_ERR_NOT_INITIALIZED,
	TOKEN_ERR_BAD_ARGS,
	TOKEN_ERR_SAFE,
	TOKEN_ERR_MECH,
	TOKEN_ERR_MECH_NOT_FOUND,
} token_error_t;

token_t *token_new(unsigned char man_id[MAN_ID_SIZE],
		   unsigned char model[MODEL_SIZE],
		   unsigned char serial[SERIAL_SIZE], version_t fw_version,
		   version_t hw_version);
void token_free(token_t *token);

token_error_t token_init(token_t *token, unsigned char label[LABEL_SIZE],
			 unsigned char *pin, unsigned long pin_len);
token_error_t token_get_info(token_t *token, token_info_t *info);
token_error_t token_get_mech(token_t *token, unsigned long mech_type,
			     mech_t **mech);
token_error_t token_get_mechs_len(token_t *token, unsigned long *mechs_len);
token_error_t token_get_mech_list(token_t *token, unsigned long *mech_list,
				  unsigned long mechs_len);

#endif // BEARCLAVE_TOKEN_H
