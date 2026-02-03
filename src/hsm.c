#include "hsm.h"
#include "safe.h"

#include <string.h>

hsm_t g_hsm = {.initialized = false};

const char g_ck_version_major = '3';
const char g_ck_version_minor = '0';
const char g_lib_version_major = '0';
const char g_lib_version_minor = '1';
const char g_pad_val = ' ';
const char *const g_man_id = "Bearclave";
const char *const g_lib_desc = "Bearclave PKCS#11 HSM Emulator";
const unsigned long int g_flags = 0;

hsm_error_t hsm_initialize(void) {
	if (g_hsm.initialized) {
		return HSM_OK;
	}
	g_hsm.initialized = true;
	return HSM_OK;
}

hsm_error_t hsm_finalize(void) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	g_hsm.initialized = false;
	return HSM_OK;
}

hsm_error_t hsm_get_ck_version_major(unsigned char *major) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	if (major == NULL) {
		return HSM_ERR_BAD_ARGS;
	}
	*major = g_ck_version_major;
	return HSM_OK;
}

hsm_error_t hsm_get_ck_version_minor(unsigned char *minor) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	if (minor == NULL) {
		return HSM_ERR_BAD_ARGS;
	}
	*minor = g_ck_version_minor;
	return HSM_OK;
}

hsm_error_t hsm_get_flags(unsigned long *flags) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	if (flags == NULL) {
		return HSM_ERR_BAD_ARGS;
	}
	*flags = g_flags;
	return HSM_OK;
}

hsm_error_t hsm_get_lib_desc(unsigned char *lib_desc, size_t lib_desc_len) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	if (lib_desc == NULL) {
		return HSM_ERR_BAD_ARGS;
	}

	// Copy over our library description along with any necessary padding.
	// According to pkcs11t.h, it should be blank-space padded.
	const size_t desc_len = strlen(g_lib_desc);
	const size_t desc_pad_len = lib_desc_len - desc_len;
	int ret = safe_memcpy(lib_desc, lib_desc_len, g_lib_desc, desc_len);
	if (ret != SAFE_OK) {
		return HSM_ERR_MEMCPY;
	}
	ret = safe_memset(lib_desc + desc_len, desc_pad_len, desc_pad_len,
			  g_pad_val);
	if (ret != SAFE_OK) {
		return HSM_ERR_MEMSET;
	}
	return HSM_OK;
}

hsm_error_t hsm_get_lib_version_major(unsigned char *major) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	if (major == NULL) {
		return HSM_ERR_BAD_ARGS;
	}
	*major = g_lib_version_major;
	return HSM_OK;
}

hsm_error_t hsm_get_lib_version_minor(unsigned char *minor) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	if (minor == NULL) {
		return HSM_ERR_BAD_ARGS;
	}
	*minor = g_lib_version_minor;
	return HSM_OK;
}

hsm_error_t hsm_get_man_id(unsigned char *man_id, size_t man_id_len) {
	if (!g_hsm.initialized) {
		return HSM_ERR_NOT_INITIALIZED;
	}
	if (man_id == NULL) {
		return HSM_ERR_BAD_ARGS;
	}

	// Copy over our manufacturer id along with any necessary padding.
	// According to pkcs11t.h, it should be blank-space padded.
	const size_t id_len = strlen(g_man_id);
	const size_t id_pad_len = man_id_len - id_len;
	int ret = safe_memcpy(man_id, man_id_len, g_man_id, id_len);
	if (ret != SAFE_OK) {
		return ret;
	}
	ret = safe_memset(man_id + id_len, id_pad_len, id_pad_len, g_pad_val);
	if (ret != SAFE_OK) {
		return ret;
	}
	return HSM_OK;
}
