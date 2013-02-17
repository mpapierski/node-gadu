#if !defined(UV_RESOLVER_H)
#define UV_RESOLVER_H

#include <cstdlib>
#include <unistd.h>
#include <uv.h>
#include "helpers.h"

struct uv_resolve_data {
	// Default loop (just to save a single call!)
	uv_loop_t * loop;
	// Resolve data
	uv_getaddrinfo_t resolve;
	// Pipes
	int pipes[2];
};

/**
 * Start DNS resolve using libuv's async dns resolver.
 */
int uv_resolver_start(int* fd, void** priv_data, const char* name);

/**
 * Cleanup resolver data. Called when the name is resolved.
 */
void uv_resolver_cleanup(void** priv_data, int force);

#endif