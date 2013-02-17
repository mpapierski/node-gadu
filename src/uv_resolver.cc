#include <cstdio>
#include "uv_resolver.h"

static void on_resolved(uv_getaddrinfo_t * resolver, int status, struct addrinfo * res) {
	uv_resolve_data * data = reinterpret_cast<uv_resolve_data *>(resolver->data);
	if (status == -1) {
		const char * error = uv_err_name(uv_last_error(data->loop));
		(void)error;
		return;
	}
	// Get IP from host
	char addr[17] = { '\n' };
	uv_ip4_name(reinterpret_cast<struct sockaddr_in*>(res->ai_addr), addr, 16);
	uv_freeaddrinfo(res);
	// Send IP back
	struct in_addr addr_ip[2];
	int addr_count = 1;
	inet_pton(AF_INET, addr, &addr_ip[0]);
	
	if (send(data->pipes[1], addr_ip, (addr_count + 1) * sizeof(struct in_addr), 0) != (int)((addr_count + 1) * sizeof(struct in_addr))) {
		// Unable to send data?
	}
}

int uv_resolver_start(int* fd, void** priv_data, const char* name) {
	uv_resolve_data * data = new uv_resolve_data();
	data->loop = uv_default_loop();
	// Set up pipes.
	if (socketpair(AF_LOCAL, SOCK_STREAM, 0, data->pipes) == -1) {
		return -1;
	}
	// Spawn new UV resolver/
	data->resolve.data = data;
	int r = uv_getaddrinfo(data->loop, &data->resolve, on_resolved, name, NULL, NULL);
	if (r < 0) {
		// Unable to start DNS resolver.
		return r;
	}
	*fd = data->pipes[0];
	*priv_data = data;
	return 0;
}

void uv_resolver_cleanup(void** priv_data, int force) {
	uv_resolve_data * data = reinterpret_cast<uv_resolve_data *>(*priv_data);
	delete data;
}