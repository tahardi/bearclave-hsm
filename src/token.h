#ifndef BEARCLAVE_TOKEN_H
#define BEARCLAVE_TOKEN_H
#include "common.h"

typedef struct token token_t;

typedef struct {
	unsigned char label[TOKEN_LABEL_SIZE];
	unsigned char manufacturer_id[MAN_ID_SIZE];
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
} token_error_t;

token_t *token_new(unsigned char *label, unsigned char *manufacturer_id,
		   unsigned char *model, unsigned char *serial);
void token_free(token_t *token);

token_error_t token_get_info(token_t *token, token_info_t *info);

#endif // BEARCLAVE_TOKEN_H
