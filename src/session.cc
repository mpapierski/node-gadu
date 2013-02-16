#include <node.h>
#include <cerrno>
#include <iostream>
#include <cstring>
#include "session.h"

using namespace v8;

Session::Session()
	: session_(0)
	, poll_fd_(0)
	, last_(0) {
}
Session::~Session() {
	if (!session_)
		return;
	::gg_free_session(session_);
}

void Session::Init(Handle<Object> target) {
	gg_debug_level = 255;
	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
	tpl->SetClassName(String::NewSymbol("Session"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	// Prototype
	tpl->PrototypeTemplate()->Set(String::NewSymbol("login"),
		FunctionTemplate::New(Login)->GetFunction());

	Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
	target->Set(String::NewSymbol("Session"), constructor);
}


Handle<Value> Session::New(const Arguments& args) {
	HandleScope scope;
	Session * obj = new Session();
  	obj->Wrap(args.This());
	return args.This();
}

v8::Handle<v8::Value> Session::Login(const v8::Arguments& args) {
	HandleScope scope;
	Session * obj = ObjectWrap::Unwrap<Session>(args.This());
	struct gg_login_params * p = new struct gg_login_params;
	memset(p, 0, sizeof(struct gg_login_params));
	p->uin = args[0]->NumberValue();
	p->password = *v8::String::AsciiValue(args[1]->ToString());
	p->async = 1;
	struct ::gg_session * sess = ::gg_login(p);
	if (!sess) {
		const char * error = strerror(errno);
		return scope.Close(v8::ThrowException(v8::String::New(error)));
	}
	if (gg_session_set_resolver(sess, GG_RESOLVER_PTHREAD) < 0) {
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
	obj->now_ = time(NULL);
	return scope.Close(args.This());
}

void Session::gadu_perform(uv_poll_t *req, int status, int events) {
	Session * obj = static_cast<Session *>(req->data);
	
	struct gg_session * sess = obj->session_;

  	obj->now_ = time(NULL);

    if (obj->now_ != obj->last_) {
            if (sess->timeout != -1 && sess->timeout-- == 0 && !sess->soft_timeout) {
                    printf("Przekroczenie czasu operacji.\n");
                    gg_free_session(sess);
                    return; //1;
            }
    }

 	if (sess && ((events & UV_READABLE) || (events & UV_WRITABLE) || (sess->timeout == 0 && sess->soft_timeout)))
 	{
 		struct gg_event * e = gg_watch_fd(sess);
        /*if (!(e = gg_watch_fd(sess))) {
                std::cout << ("Połączenie zerwane.\n") << std::endl;
                gg_free_session(sess);
                //return 1;
        }

        if (e->type == GG_EVENT_CONN_SUCCESS) {
                printf("Połączono z serwerem.\n");
                gg_free_event(e);
                gg_logoff(sess);
                gg_free_session(sess);
                //return 0;
        }

        if (e->type == GG_EVENT_CONN_FAILED) {
                printf("Błąd połączenia.\n");
                gg_free_event(e);
                gg_logoff(sess);
                gg_free_session(sess);
               // return 1;
        }*/
       	gg_free_event(e);
	}
}