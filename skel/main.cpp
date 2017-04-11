#include "main.h"

namespace skel {
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::Number;
	using v8::Value;
	using v8::String;

	/*
	* GENERAL FUNCTIONS
	*/
	string getString(const FunctionCallbackInfo<Value>& args, int index) {
		String::Utf8Value utfValue(args[index]);
		return string(*utfValue);
	}
	char *getChar(const FunctionCallbackInfo<Value>& args, int index) {
		string str = getString(args, index);
		char *chr = new char[str.size() + 1];
		//char *chr = (char *)alloca(str.size() + 1);
		memcpy(chr, str.c_str(), str.size() + 1);
		return chr;
	}
	bool getBool(const FunctionCallbackInfo<Value>& args, int index) {
		return args[index]->IsUndefined() ? false : args[index]->BooleanValue();
	}
	int getInt(const FunctionCallbackInfo<Value>& args, int index) {
		return (int)args[index]->IsUndefined() ? 0 : args[index]->Int32Value();
	}
	int getUInt(const FunctionCallbackInfo<Value>& args, int index) {
		return (unsigned int)args[index]->IsUndefined() ? 0 : args[index]->Uint32Value();
	}


	/*
	* API FUNCTIONS
	*/
	void hello(const FunctionCallbackInfo<Value>& args) {
		printf("Hello world\n");
	}

	void init(Local<Object> exports) {
		NODE_SET_METHOD(exports, "hello", hello);
		MyObject::Init(exports);
	}

	NODE_MODULE(skel, init)
}