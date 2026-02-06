#ifndef BEARCLAVE_MECHANISM_H
#define BEARCLAVE_MECHANISM_H

typedef struct mech mech_t;

typedef struct {
	unsigned long flags;
	unsigned long ulMaxKeySize;
	unsigned long ulMinKeySize;
	unsigned long type;
} mech_info_t;

typedef enum { MECH_OK = 0, MECH_ERR_BAD_ARGS } mech_error_t;

mech_t *mech_new(unsigned long type);
void mech_free(mech_t *mech);

mech_error_t mech_get_info(mech_t *mech, mech_info_t *info);

#endif // BEARCLAVE_MECHANISM_H
