#ifndef BEARCLAVE_HSM_H
#define BEARCLAVE_HSM_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
	bool initialized;
} hsm_t;

typedef enum {
	HSM_OK = 0,
	HSM_ERR_MEMCPY = 1,
	HSM_ERR_MEMSET = 2,
	HSM_ERR_NOT_INITIALIZED = 3,
	HSM_ERR_BAD_ARGS = 4,
} hsm_error_t;

extern hsm_t g_hsm;

extern const char g_ck_version_major;
extern const char g_ck_version_minor;
extern const char g_lib_version_major;
extern const char g_lib_version_minor;
extern const char g_pad_val;
extern const char *const g_man_id;
extern const char *const g_lib_desc;
extern const unsigned long int g_flags;

hsm_error_t hsm_initialize(void);
hsm_error_t hsm_finalize(void);
hsm_error_t hsm_get_ck_version_major(unsigned char *major);
hsm_error_t hsm_get_ck_version_minor(unsigned char *minor);
hsm_error_t hsm_get_flags(unsigned long *flags);
hsm_error_t hsm_get_lib_desc(unsigned char *lib_desc, size_t lib_desc_len);
hsm_error_t hsm_get_lib_version_major(unsigned char *major);
hsm_error_t hsm_get_lib_version_minor(unsigned char *minor);
hsm_error_t hsm_get_man_id(unsigned char *man_id, size_t man_id_len);

#endif // BEARCLAVE_HSM_H
