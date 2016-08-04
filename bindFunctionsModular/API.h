#ifndef _API_H
#define _API_H

#include "include/v8.h"
#include "include/libplatform/libplatform.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include "variables.h"

using namespace v8;

Handle<String> GetScriptFromJSCode(Isolate* isolate, std::string JScode);

Handle<String> GetScriptFromFile(Isolate* isolate, std::string fileName);

void GetParameter(const FunctionCallbackInfo<Value>& args);

void SetParameter(const FunctionCallbackInfo<Value>& args);

void Print(const FunctionCallbackInfo<Value>& args);

void PrintParameter(const FunctionCallbackInfo<Value>& args);

class Functions {
	public:
		std::string GetParameterValue(std::string parameter);
		void SetParameterValue(std::string parameter, std::string value);
		void PrintParameterValue(std::string parameter);
};

extern Functions f;
#endif // _API_H
