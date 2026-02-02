#ifndef BEARCLAVE_HSM_H
#define BEARCLAVE_HSM_H

#include <stdbool.h>
#include <stdint.h>

// This is just an example of what your HSM state might look like.
// Adding this here as more of a stub that will be updated as
// we start building out pkcs11 function implementations.
typedef struct {
	uint32_t next_session_handle;
	uint32_t next_object_handle;

	void *sessions; // Dynamic data structure (hash table, tree, etc.)
	void *objects;	// Store keys, certs, etc.
	void *slots;	// Slot information
	bool initialized;
} HSM;

// Global state instance
extern HSM g_hsm;

#endif // BEARCLAVE_HSM_H
