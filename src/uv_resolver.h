#if !defined(UV_RESOLVER_H)
#define UV_RESOLVER_H

/**
 * Structure for holding temporary data used for DNS queries.
 */
struct uv_resolver_data_t {
	// DNS resolve result is sent by pipe.
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