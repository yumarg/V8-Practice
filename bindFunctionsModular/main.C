#include "v8engine.h"
#include "API.h"
#include "variables.h"
#include <iostream>
using namespace v8;

V8Instance* v8instance;

int main(int argc, char** argv) {
	
	// Initialize V8.
	v8instance = new V8Instance((char*) argv[0]);
	std::cout << "..V8 initialized.." << std::endl;



	// Set up JavaScript execution environment.

	Isolate* isolate = v8instance->GetIsolate();
        Isolate::Scope isolate_scope(isolate);
        HandleScope handle_scope(isolate);

        isolate->Enter();

        // Compile and run JS.
        std::cout << "\n..setting up JS execution environment..\n" << std::endl;
	Local<Script> script = v8instance->SetUpJSExecutionEnvironment("bindFunctions.js");

	std::cout << "\n..compiling and executing JS..\n" << std::endl;
	Local<Value> result = v8instance->Execute(script);

        std::cout << "\n..JS executed..\n" << std::endl;	

        // Change all parameter values in symbolTable that have been changed
        for (const auto &p : symbolTableCopy) {
                if (p.second != symbolTable.at(p.first)) {
                        symbolTable[p.first] = p.second;
                }
        }
 


	// Tear down V8.
	delete v8instance;
	v8instance = nullptr;
}
