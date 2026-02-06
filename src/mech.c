#include "mechanism.h"
#include "common.h"

#include <stdlib.h>

typedef struct mech {
	mech_info_t *info;
} mech_t;

mech_t *mech_new(unsigned long type) {
	mech_t *mech = malloc(sizeof(mech_t));
	if (mech == NULL) {
		return NULL;
	}

	mech->info = malloc(sizeof(mech_info_t));
	if (mech->info == NULL) {
		free(mech);
		return NULL;
	}

	mech->info->type = type;
	mech->info->flags = 0;
	mech->info->ulMaxKeySize = MECH_MAX_KEY_SIZE;
	mech->info->ulMinKeySize = MECH_MIN_KEY_SIZE;
	return mech;
}

void mech_free(mech_t *mech) {
	if (mech == NULL) {
		return;
	}
	free(mech->info);
	free(mech);
}

mech_error_t mech_get_info(mech_t *mech, mech_info_t *info) {
	if (mech == NULL || info == NULL) {
		return MECH_ERR_BAD_ARGS;
	}

	info->type = mech->info->type;
	info->flags = mech->info->flags;
	info->ulMaxKeySize = mech->info->ulMaxKeySize;
	info->ulMinKeySize = mech->info->ulMinKeySize;
	return MECH_OK;
}
