#include "bearclave/hsm.h"
#include "bearclave/pkcs11.h"

HSM g_hsm = {.next_session_handle = 1,
	     .next_object_handle = 1,
	     .sessions = NULL_PTR,
	     .objects = NULL_PTR,
	     .slots = NULL_PTR,
	     .initialized = false};
