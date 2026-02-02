#ifndef BEARCLAVE_HSM_H
#define BEARCLAVE_HSM_H

#include <bits/pthreadtypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

// This is just an example of what your HSM state might look like.
// Adding this here as more of a stub that will be updated as
// we start building out pkcs11 function implementations.
typedef struct {
	uint32_t next_session_handle;
	uint32_t next_object_handle;

	// Add other global state here
	void *sessions;      // Dynamic data structure (hash table, tree, etc.)
	void *objects;       // Store keys, certs, etc.
	void *slots;         // Slot information
	bool initialized;

	// Consider thread safety
	pthread_mutex_t mutex;
} HSM;

// Global state instance
extern HSM g_hsm;

#endif // BEARCLAVE_HSM_H