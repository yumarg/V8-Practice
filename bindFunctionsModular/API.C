#include "API.h"

using namespace v8;

Functions f;

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

std::string Functions::GetParameterValue(std::string parameter) { return symbolTableCopy.at(parameter); }

void Functions::SetParameterValue(std::string parameter, std::string value) { symbolTableCopy[parameter] = value; }

void Functions::PrintParameterValue(std::string parameter) { std::cout << symbolTableCopy.at(parameter); }

void GetParameter(const FunctionCallbackInfo<Value>& args) {
        if (args.Length() < 1) return;
        HandleScope scope(args.GetIsolate());
        Local<Value> arg = args[0];
        String::Utf8Value value(arg);
        f.GetParameterValue(*value);
}

void SetParameter(const FunctionCallbackInfo<Value>& args) {
        if (args.Length() < 2) return;
        HandleScope scope(args.GetIsolate());
        Local<Value> param = args[0];
        String::Utf8Value value(param);
        std::string p = *value;
        Local<Value> newValue = args[1];
        String::Utf8Value str(newValue);
        std::string v = *str;
        f.SetParameterValue(p, v);
}

void Print(const FunctionCallbackInfo<Value>& args) {
	HandleScope scope(args.GetIsolate());
	for (int i = 0; i < args.Length(); i++) {
		String::Utf8Value arg(args[i]);
		std::cout << *arg << " ";
	}
}

void PrintParameter(const FunctionCallbackInfo<Value>& args) {
	HandleScope scope(args.GetIsolate());
	Local<Value> arg = args[0];
	String::Utf8Value value(arg);
	f.PrintParameterValue(*value);
}
