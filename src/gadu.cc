#include <node.h>
#include <v8.h>
#include "libgadu.h"
#include "session.h"

using namespace v8;

/**
 * Return version of libgadu.
 */
Handle<Value> Version(const Arguments& args) {
	HandleScope scope;
	return scope.Close(String::New(::gg_libgadu_version()));
}

extern "C" {

/**
 * Module initializer
 */
void init(Handle<Object> target) {
	NODE_SET_METHOD(target, "version", Version);
	// Events
	NODE_DEFINE_CONSTANT(target, GG_EVENT_CONN_SUCCESS);
	NODE_DEFINE_CONSTANT(target, GG_EVENT_CONN_FAILED);
	NODE_DEFINE_CONSTANT(target, GG_EVENT_MSG);
	// Ack defines
	NODE_DEFINE_CONSTANT(target, GG_ACK_BLOCKED);
	NODE_DEFINE_CONSTANT(target, GG_ACK_DELIVERED);
	NODE_DEFINE_CONSTANT(target, GG_ACK_QUEUED);
	NODE_DEFINE_CONSTANT(target, GG_ACK_MBOXFULL);
	NODE_DEFINE_CONSTANT(target, GG_ACK_NOT_DELIVERED);
	// Message classes
	NODE_DEFINE_CONSTANT(target, GG_CLASS_QUEUED);
	NODE_DEFINE_CONSTANT(target, GG_CLASS_OFFLINE);
	NODE_DEFINE_CONSTANT(target, GG_CLASS_MSG);
	NODE_DEFINE_CONSTANT(target, GG_CLASS_CHAT);
	NODE_DEFINE_CONSTANT(target, GG_CLASS_CTCP);
	NODE_DEFINE_CONSTANT(target, GG_CLASS_ACK);
	NODE_DEFINE_CONSTANT(target, GG_CLASS_EXT);
	Session::Init(target);
}

}

NODE_MODULE(binding, init);