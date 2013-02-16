#if !defined(UV_RESOLVER_H)
#define UV_RESOLVER_H

#include <unistd.h>

/**
 * Start DNS resolve using libuv's async dns resolver.
 */
int uv_resolver_start(int* fd, void** priv_data, const char* name);

/**
 * Cleanup resolver data. Called when the name is resolved.
 */
void uv_resolver_cleanup(void** priv_data, int force);

#endif