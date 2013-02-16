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
	NODE_DEFINE_CONSTANT(target, GG_EVENT_CONN_SUCCESS);
	NODE_DEFINE_CONSTANT(target, GG_EVENT_CONN_FAILED);
	NODE_DEFINE_CONSTANT(target, GG_EVENT_MSG);
	Session::Init(target);
}

}

NODE_MODULE(binding, init);