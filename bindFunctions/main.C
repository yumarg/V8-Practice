#include "v8.h"
#include "libplatform/libplatform.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <map>

using namespace v8;


std::map<std::string, std::string> symbolTable = {
        {"Param1", "p1"},
        {"Param2", "p2"},
        {"Param3", "p3"}
};

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
        public:
                virtual void* Allocate(size_t length) {
                        void* data = AllocateUninitialized(length);
                        return data == NULL ? data : memset(data, 0, length);
                }
                virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
                virtual void Free(void* data, size_t) { free(data); }
};

Handle<String> GetScriptFromFile(Isolate* isolate, std::string fileName) {
        std::string src;
        std::string line;
        std::ifstream file(fileName);
        while (std::getline(file, line)) {
                src += line;
        }
        const char *js = src.c_str();
        return String::NewFromUtf8(isolate, js, NewStringType::kNormal).ToLocalChecked();
}

Handle<String> GetScriptFromJSCode(Isolate* isolate, std::string JScode) {
	const char *js = JScode.c_str();
	return String::NewFromUtf8(isolate, js, NewStringType::kNormal).ToLocalChecked();
}

class Functions {
	public:
		std::string GetParameterValue(std::string parameter) {
			return symbolTable.at(parameter);
		}
		void SetParameterValue(std::string parameter, std::string value) {
			symbolTable[parameter] = value;
		}
};

static void GetParameter(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() < 1) return;
	HandleScope scope(args.GetIsolate());
	Local<Value> arg = args[0];
	String::Utf8Value value(arg);
	Functions f;
	f.GetParameterValue(*value);
	std::cout << f.GetParameterValue(*value) << std::endl;
}

static void SetParameter(const FunctionCallbackInfo<Value>& args) {
	if (args.Length() < 2) return;
	HandleScope scope(args.GetIsolate());
	Local<Value> param = args[0];
	String::Utf8Value value(param);
	std::string p = *value;
	Local<Value> newValue = args[1];
	String::Utf8Value str(newValue);
	std::string v = *str;
	Functions f;
	f.SetParameterValue(p, v);
}
	

int main(int argc, const char *argv[]) {
        // Initialize V8.
        V8::InitializeICU();
        V8::InitializeExternalStartupData(argv[0]);
        Platform* platform = platform::CreateDefaultPlatform();
        V8::InitializePlatform(platform);
        V8::Initialize();

        std::cout << "..V8 initialized.." << std::endl;

        // Create new isolate
        ArrayBufferAllocator allocator;
        Isolate::CreateParams create_params;
        create_params.array_buffer_allocator = &allocator;
        Isolate* isolate = Isolate::New(create_params);

        std::cout << "..new isolate created..\n" << std::endl;

        {
                Isolate::Scope isolate_scope(isolate);
                HandleScope handle_scope(isolate);
                Local<Context> context = Context::New(isolate);
                Context::Scope context_scope(context);
                // Compile and run JS
                //Handle<String> source = GetScriptFromFile(isolate, "simpleFunctions.js");
                //Script::Compile(source)->Run(context).ToLocalChecked();

                //std::cout << "..JS file w/function definitions compiled and run..\n" << std::endl;

                Handle<Object> global = context->Global();

		// API
		global->Set(String::NewFromUtf8(isolate, "getParameterValue", NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(isolate, GetParameter)->GetFunction());
		global->Set(String::NewFromUtf8(isolate, "setParameterValue", NewStringType::kNormal).ToLocalChecked(), FunctionTemplate::New(isolate, SetParameter)->GetFunction());

		// Compile and run JS
		Handle<String> source = GetScriptFromFile(isolate, "simpleBindingFunctions.js");
		Handle<Value> result = Script::Compile(source)->Run(context).ToLocalChecked();

                std::cout << "\n..JS executed..\n" << std::endl;

		std::cout << "..symbolTable after executing JS.." << std::endl;
		for (const auto &p : symbolTable) {
			std::cout <<  p.first << " : " << p.second << std::endl;
		}

		std::cout << std::endl;

                //String::Utf8Value str(result);
                //std::cout << "..result:.. " << std::endl;
                //printf("%s\n", *str);
                //std::cout << "..end of result..\n" << std::endl;

        }

        // Dispose isolate and tear down V8
        isolate->Dispose();
        V8::Dispose();
        V8::ShutdownPlatform();
        delete platform;

        return 0;
}
