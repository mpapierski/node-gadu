#include <node.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include "session.h"
#include "helpers.h"
#include "uv_resolver.h"

using namespace v8;

Session::Session()
	: session_(0)
	, poll_fd_(0)
	, timer_poll_(0) {
}
Session::~Session() {
}


Persistent<Function> Session::constructor;

void Session::Init(Handle<Object> target) {
	// gg_debug_level = 255;
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);

	tpl->SetClassName(String::NewSymbol("Session"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype

	NODE_SET_PROTOTYPE_METHOD(tpl, "login", Login);
	NODE_SET_PROTOTYPE_METHOD(tpl, "logoff", Logoff);
	NODE_SET_PROTOTYPE_METHOD(tpl, "send", SendMessage);
	NODE_SET_PROTOTYPE_METHOD(tpl, "notify", Notify);

	constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(String::NewSymbol("Session"), constructor);
}


Handle<Value> Session::New(const Arguments& args) {
	HandleScope scope;
	Session * obj = new Session();
	obj->Wrap(args.This());

	// Install global DNS resolver
	if (gg_global_set_custom_resolver(uv_resolver_start, uv_resolver_cleanup) < 0) {
		const char * error = strerror(errno);
		return ThrowException(String::New(error));
	}

	return args.This();
}

v8::Handle<v8::Value> Session::Login(const v8::Arguments& args) {
	HandleScope scope;
	Session * obj = ObjectWrap::Unwrap<Session>(args.This());
	struct gg_login_params p;
	memset(&p, 0, sizeof(struct gg_login_params));
	p.uin = args[0]->NumberValue();
	p.password = *v8::String::AsciiValue(args[1]->ToString());
	p.async = 1;
	// Save persistent callback
	obj->login_callback_ = Persistent<Function>::New(Local<Function>::Cast(args[2]));
	// Do login.
	struct ::gg_session * sess = ::gg_login(&p);
	if (!sess) {
		const char * error = strerror(errno);
		return v8::ThrowException(v8::String::New(error));
	}
	obj->session_ = sess;
	// Start polling
	obj->poll_fd_ = static_cast<uv_poll_t*>(malloc(sizeof(uv_poll_t)));
	uv_poll_init(uv_default_loop(), obj->poll_fd_, sess->fd);
	obj->poll_fd_->data = obj;
	// Setup ping timer.
	obj->timer_poll_ = new uv_timer_t;
	uv_timer_init(uv_default_loop(), obj->timer_poll_);
	obj->timer_poll_->data = obj;
	uv_timer_start(obj->timer_poll_, ping_callback, 0, 60000);

	// Watch for R/W
	if ((sess->check & GG_CHECK_READ))
		uv_poll_start(obj->poll_fd_, UV_READABLE, gadu_perform);
	if ((sess->check & GG_CHECK_WRITE))
		uv_poll_start(obj->poll_fd_, UV_WRITABLE, gadu_perform);
	return args.This();
}

v8::Handle<v8::Value> Session::SendMessage(const v8::Arguments& args) {
	HandleScope scope;
	Session * obj = ObjectWrap::Unwrap<Session>(args.This());
	unsigned long uin = args[0]->NumberValue();
	unsigned char * text = reinterpret_cast<unsigned char*>(*v8::String::AsciiValue(args[1]->ToString()));
	if (gg_send_message(obj->session_, GG_CLASS_MSG, uin, text) < 0) {
		obj->disconnect();
	}
	return args.This();
}

v8::Handle<v8::Value> Session::Notify(const v8::Arguments& args) {
	HandleScope scope;
	Session * obj = ObjectWrap::Unwrap<Session>(args.This());
	struct gg_session * sess = obj->session_;

	// Convert v8::Array of Numbers to std::vector.
	std::vector<uin_t> contacts;
	if ((args.Length() == 1) && args[0]->IsArray()) {
		Local<Array> values = Local<Array>::Cast(args[0]);
		contacts.resize(values->Length());
		for (unsigned int i = 0; i < values->Length(); i++) {
			if (values->CloneElementAt(i)->IsNumber()) {
				return ThrowException(Exception::TypeError(String::New("Invalid uin")));
			}
			contacts[i] = values->CloneElementAt(i)->NumberValue();
		}
	}
	// Notify server with contact list.
	if (gg_notify(sess, contacts.data(), contacts.size()) < 0) {
		obj->disconnect();
	}
	return args.This();
}

v8::Handle<v8::Value> Session::Logoff(const v8::Arguments& args) {
	HandleScope scope;
	Session * obj = ObjectWrap::Unwrap<Session>(args.This());
	struct gg_session * sess = obj->session_;
	gg_logoff(sess);
	uv_poll_stop(obj->poll_fd_);
	uv_close((uv_handle_t*)obj->poll_fd_, (uv_close_cb)free);
	uv_timer_stop(obj->timer_poll_);
	uv_close((uv_handle_t*)obj->timer_poll_, (uv_close_cb)free);
	gg_free_session(sess);
	return args.This();
}

void Session::gadu_perform(uv_poll_t *req, int status, int events) {
	Session * obj = static_cast<Session *>(req->data);
	struct gg_session * sess = obj->session_;
	
	if (sess && ((events & UV_READABLE) || (events & UV_WRITABLE) || (sess->timeout == 0 && sess->soft_timeout))) {	
		struct gg_event * e = 0;
		raii_destructor<struct gg_event> destructor(e, &gg_free_event);
		
		if (!(e = gg_watch_fd(sess))) {
			// In case of error, event value passed is Undefined.
			Local<Value> argv[1] = { Local<Value>::New(Undefined()) };
			obj->login_callback_->Call(Context::GetCurrent()->Global(), 1, argv);
			obj->disconnect();
			return;
		}
		
		// Construct a new object with the events data.
		Local<Object> event = Object::New();

		NODE_SET_ATTRIBUTE(event, "type", Number::New(e->type));

		Local<Object> target = Object::New();
		switch (e->type) {
		case GG_EVENT_CONN_FAILED:
			gg_logoff(sess);
			obj->disconnect();
			return;
		case GG_EVENT_MSG: {
			// Received message.

			NODE_SET_ATTRIBUTE(target, "sender", Number::New(e->event.msg.sender));
			NODE_SET_ATTRIBUTE(target, "msgclass", Number::New(e->event.msg.msgclass));
			NODE_SET_ATTRIBUTE(target, "time", Number::New(e->event.msg.time));
			// TODO:
			// recipients_count
			// recipients
			// formats_length
			// formats
			// seq
			NODE_SET_ATTRIBUTE(target, "seq", Number::New(e->event.msg.seq));
			char * xhtml_message = reinterpret_cast<char*>(e->event.msg.xhtml_message);
			NODE_SET_ATTRIBUTE(target, "xhtml_message", !xhtml_message ? Null() : String::New(xhtml_message));
			char * message = reinterpret_cast<char*>(e->event.msg.message);
			NODE_SET_ATTRIBUTE(target, "message", !message ? Null() : String::New(message));
			break;
		}
		default:
			break;
		}
		// Add target event details to the event object.
		event->Set(String::NewSymbol("target"), target);
		// Call the callback with newly created object.
		Local<Value> argv[1] = { Local<Value>::New(event) };
		obj->login_callback_->Call(Context::GetCurrent()->Global(), 1, argv);
		
	}
	// Watch for R/W again
	if ((sess->check & GG_CHECK_READ))
		uv_poll_start(obj->poll_fd_, UV_READABLE, gadu_perform);
	if ((sess->check & GG_CHECK_WRITE))
		uv_poll_start(obj->poll_fd_, UV_WRITABLE, gadu_perform);
}

void Session::ping_callback(uv_timer_t * timer, int status) {
	Session * obj = static_cast<Session *>(timer->data);
	if (gg_ping(obj->session_) < 0) {
		return;
	}	
}

void Session::disconnect() {
	uv_poll_stop(poll_fd_);
	uv_close((uv_handle_t*)poll_fd_, (uv_close_cb)free);
	uv_timer_stop(timer_poll_);
	uv_close((uv_handle_t*)timer_poll_, (uv_close_cb)free);
}