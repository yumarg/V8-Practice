#ifndef _V8_ENGINE_H
#define _V8_ENGINE_H

#include "include/v8.h"
#include "include/libplatform/libplatform.h"
#include <string>

using namespace v8;

class V8Instance {
	public:
		V8Instance(char *vv); // constructor
		~V8Instance(); // deconstructor

		Isolate* GetIsolate();
		Platform* GetPlatform();
		Local<Context> GetContext();
		Handle<Object> GetGlobal();
		Local<Script> SetUpJSExecutionEnvironment(std::string fileName);
		Local<Value> Execute(Local<Script> script);

	private:
		class ArrayBufferAllocator;

		ArrayBufferAllocator* v8allocator;
		Platform* v8platform;
		Isolate* v8isolate;
		Local<Context> v8context;
		Handle<Object> v8global;
};

extern V8Instance* v8instance;

#endif // _V8_ENGINE_H
