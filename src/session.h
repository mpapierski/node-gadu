#ifndef SESSION_H
#define SESSION_H

#include <vector>
#include <ctime>
#include <map>
#include <node.h>
#include "libgadu.h"

class Session : public node::ObjectWrap {
public:
	static void Init(v8::Handle<v8::Object> target);

private:
	static v8::Persistent<v8::Function> constructor;
	Session();
	~Session();

	static v8::Handle<v8::Value> New(const v8::Arguments& args);
	/**
	 * Connect to a server.
	 */
	static v8::Handle<v8::Value> Login(const v8::Arguments& args);

	/**
	 * Send message.
	 */
	static v8::Handle<v8::Value> SendMessage(const v8::Arguments& args);

	/**
	 * Notify with contact list
	 */
	static v8::Handle<v8::Value> Notify(const v8::Arguments& args);
	/**
	 * Logoff from server
	 */
	static v8::Handle<v8::Value> Logoff(const v8::Arguments& args);
	/**
	 * Low-level handle.
	 */
	struct gg_session * session_;
	/**
	 * Login params.
	 */
	struct gg_login_params login_params_;
	/**
	 * FD poller
	 */
	uv_poll_t * poll_fd_;

	/**
	 * Poller handler
	 */
	static void gadu_perform(uv_poll_t *req, int status, int events);

	/**
	 * Login callback.
	 */
	v8::Persistent<v8::Function> login_callback_;

	/**
	 * Ping timer poller
	 */
	uv_timer_t * timer_poll_;

	/**
	 * Ping timer callback
	 */
	static void ping_callback(uv_timer_t *, int status);

	/**
	 * Disconnect session.
	 */
	void disconnect();
	/**
	 * Outgoing messages callbacks. Maps Seq to Callbacks.
	 */
	std::map<int, v8::Persistent<v8::Function> > outgoing_messages_;
};

#endif