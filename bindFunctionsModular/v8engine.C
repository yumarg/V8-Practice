#include "v8engine.h"
#include "API.h"
#include "variables.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace v8;

class V8Instance :: ArrayBufferAllocator : public ArrayBuffer::Allocator {
	public:
		virtual void* Allocate(size_t length) {
			void* data = AllocateUninitialized(length);
			return data == NULL ? data : memset(data, 0, length);
		}
		virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
		virtual void Free(void* data, size_t) { free(data); }
};

V8Instance :: V8Instance(char *vv) {
	V8::InitializeICU();
	V8::InitializeExternalStartupData(vv);
	v8platform = platform::CreateDefaultPlatform();
	V8::InitializePlatform(v8platform);
	V8::Initialize();

	v8allocator = new ArrayBufferAllocator;
	Isolate::CreateParams create_params;
	create_params.array_buffer_allocator = v8allocator;
	v8isolate = Isolate::New(create_params);

	Isolate::Scope isolate_scope(v8isolate);
	HandleScope handle_scope(v8isolate);
	v8context = Context::New(v8isolate);
	Context::Scope context_scope(v8context);
	v8global = v8context->Global();

	v8isolate->Enter();
	v8context->Enter();

	//use API
	v8global->Set(String::NewFromUtf8(v8isolate, "getParameterValue", NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(v8isolate, GetParameter)->GetFunction());
	v8global->Set(String::NewFromUtf8(v8isolate, "setParameterValue", NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(v8isolate, SetParameter)->GetFunction());
	v8global->Set(String::NewFromUtf8(v8isolate, "print", NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(v8isolate, Print)->GetFunction());
	v8global->Set(String::NewFromUtf8(v8isolate, "printParameterValue", NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(v8isolate, PrintParameter)->GetFunction());
	
}

V8Instance :: ~V8Instance() {
	v8isolate->Exit();
	v8isolate = nullptr;
        V8::Dispose();
        V8::ShutdownPlatform();
        delete v8platform;
	v8platform = nullptr;
	std::cout << "..V8 exited.." << std::endl;
}

Isolate* V8Instance :: GetIsolate() { return v8isolate; }

Platform* V8Instance :: GetPlatform() { return v8platform; }

Local<Context> V8Instance :: GetContext() { return v8context; }

Handle<Object> V8Instance :: GetGlobal() { return v8global; }

Local<Script> V8Instance :: SetUpJSExecutionEnvironment(std::string fileName) {
	Handle<String> source = GetScriptFromFile(v8isolate, fileName);
	return Script::Compile(source);
}

Local<Value> V8Instance :: Execute(Local<Script> script) {
	return script->Run();
}
