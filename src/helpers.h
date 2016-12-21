#if !defined(HELPERS_H)
#define HELPERS_H

/**
 * Default destructor - call delete.
 */
template <typename T>
void default_destructor(T * & ptr) {
	delete ptr;
}

/**
 * Pass a reference to a pointer, and it will be destroyed
 * at the scope exit.
 */
template <typename T>
class raii_destructor {
private:
	T * & ptr_;
	typedef void (*destructor_type)(T *);
	destructor_type destructor_;
public:
	raii_destructor(T * & ptr, destructor_type dt = &default_destructor<T>) throw()
		: ptr_(ptr)
		, destructor_(dt) {
	}
	~raii_destructor() {
		destructor_(ptr_);
	}
};

/* Useful macros */
#define NODE_SET_ATTRIBUTE(isolate, target, key, value) \
	do { \
		bool result = target->DefineOwnProperty(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, key), value, v8::PropertyAttribute::ReadOnly); \
	} while(0)
#endif
