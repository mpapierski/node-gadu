#include <node.h>
#include <cerrno>
#include <cstring>
#include "session.h"
#include "helpers.h"

using namespace v8;

Session::Session()
	: session_(0)
	, poll_fd_(0) {
}
Session::~Session() {
	if (!session_)
		return;
	::gg_free_session(session_);
}

void Session::Init(Handle<Object> target) {
	//gg_debug_level = 255;
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("Session"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	// Prototype

	NODE_SET_PROTOTYPE_METHOD(tpl, "login", Login);

	Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(String::NewSymbol("Session"), constructor);
}


Handle<Value> Session::New(const Arguments& args) {
	HandleScope scope;
	Session * obj = new Session();
	obj->Wrap(args.This());
	return scope.Close(args.This());
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
		return scope.Close(v8::ThrowException(v8::String::New(error)));
	}
	obj->session_ = sess;
	// Start polling
	obj->poll_fd_ = static_cast<uv_poll_t*>(malloc(sizeof(uv_poll_t)));
	uv_poll_init(uv_default_loop(), obj->poll_fd_, sess->fd);
	obj->poll_fd_->data = obj;
	// Watch for R/W
	if ((sess->check & GG_CHECK_READ))
		uv_poll_start(obj->poll_fd_, UV_READABLE, gadu_perform);
	if ((sess->check & GG_CHECK_WRITE))
		uv_poll_start(obj->poll_fd_, UV_WRITABLE, gadu_perform);
	return scope.Close(args.This());
}

void Session::gadu_perform(uv_poll_t *req, int status, int events) {
	Session * obj = static_cast<Session *>(req->data);
	
	struct gg_session * sess = obj->session_;

	if ((events & UV_READABLE) || (events & UV_WRITABLE))
	{
		struct gg_event * e = 0;
		raii_destructor<struct gg_event> destructor(e, &gg_free_event);

		if (!(e = gg_watch_fd(sess))) {
				// In case of error, event value passed is Undefined.
				Local<Value> argv[1] = { Local<Value>::New(Undefined()) };
				obj->login_callback_->Call(Context::GetCurrent()->Global(), 1, argv);
				gg_free_session(sess);
				uv_poll_stop(obj->poll_fd_);
				uv_close((uv_handle_t*)obj->poll_fd_, (uv_close_cb)free);
				return;
		}
		
		// Construct a new object with the events data.
		Local<Object> event = Object::New();
		event->Set(String::NewSymbol("type"),
			v8::Number::New(e->type),
			static_cast<v8::PropertyAttribute>(v8::ReadOnly|v8::DontDelete));
		// Call the callback with newly created object.
		Local<Value> argv[1] = { Local<Value>::New(event) };
		obj->login_callback_->Call(Context::GetCurrent()->Global(), 1, argv);
		if (e->type == GG_EVENT_CONN_FAILED) {
			gg_logoff(sess);
			gg_free_session(sess);
			uv_poll_stop(obj->poll_fd_);
			uv_close((uv_handle_t*)obj->poll_fd_, (uv_close_cb)free);
			return;
		}
	}
	// Watch for R/W again
	if ((sess->check & GG_CHECK_READ))
		uv_poll_start(obj->poll_fd_, UV_READABLE, gadu_perform);
	if ((sess->check & GG_CHECK_WRITE))
		uv_poll_start(obj->poll_fd_, UV_WRITABLE, gadu_perform);
}