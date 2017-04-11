#include "main.h"

namespace openal {
	using v8::Context;
	using v8::Function;
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Number;
	using v8::Array;
	using v8::Object;
	using v8::Value;
	using v8::String;
	using v8::Handle;
	using v8::Persistent;

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
		memcpy(chr, str.c_str(), str.size() + 1);
		return chr;
	}
	bool getBool(const FunctionCallbackInfo<Value>& args, int index) {
		return args[index]->IsUndefined() ? false : args[index]->BooleanValue();
	}
	int getInt(const FunctionCallbackInfo<Value>& args, int index) {
		return (int)args[index]->IsUndefined() ? 0 : args[index]->Int32Value();
	}
	unsigned int getUInt(const FunctionCallbackInfo<Value>& args, int index) {
		return (unsigned int)args[index]->IsUndefined() ? 0 : args[index]->Uint32Value();
	}
	unsigned long int getULInt(const FunctionCallbackInfo<Value>& args, int index) {
		return (unsigned long int)args[index]->IsUndefined() ? 0 : args[index]->Uint32Value();
	}
	unsigned long long int getULLInt(const FunctionCallbackInfo<Value>& args, int index) {
		return (unsigned long long int)args[index]->IsUndefined() ? 0 : args[index]->Uint32Value();
	}
	Local<Object> newDevice(const FunctionCallbackInfo<Value> &args) {
		Isolate *isolate = args.GetIsolate();
		const int argc = 1;
		Local<Value> argv[argc] = { args[0] };
		Local<Context> context = isolate->GetCurrentContext();
		Local<Function> cons = Local<Function>::New(isolate, DeviceObject::constructor);
		Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
		return result;
	}
	Local<Object> newContext(const FunctionCallbackInfo<Value> &args) {
		Isolate *isolate = args.GetIsolate();
		const int argc = 1;
		Local<Value> argv[argc] = { args[0] };
		Local<Context> context = isolate->GetCurrentContext();
		Local<Function> cons = Local<Function>::New(isolate, ContextObject::constructor);
		Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
		return result;
	}
	/*
	* API FUNCTIONS
	*/
	void getError(const FunctionCallbackInfo<Value>& args) {
		DeviceObject *device = node::ObjectWrap::Unwrap<DeviceObject>(args[0]->ToObject());
		args.GetReturnValue().Set(alcGetError(device->index));
	}
	void openDevice(const FunctionCallbackInfo<Value>& args) {
		Local<Object> result = newDevice(args);
		DeviceObject *obj = node::ObjectWrap::Unwrap<DeviceObject>(result->ToObject());
		char *str = getChar(args, 0);
		obj->index = alcOpenDevice(str);
		args.GetReturnValue().Set(result);
	}
	void createContext(const FunctionCallbackInfo<Value>& args) {
		Local<Object> result = newContext(args);
		ContextObject *obj = node::ObjectWrap::Unwrap<ContextObject>(result->ToObject());
		DeviceObject *device = node::ObjectWrap::Unwrap<DeviceObject>(args[0]->ToObject());
		obj->index = alcCreateContext(device->index, NULL);
		args.GetReturnValue().Set(result);
	}
	void makeContext(const FunctionCallbackInfo<Value>& args) {
		ContextObject *context = node::ObjectWrap::Unwrap<ContextObject>(args[0]->ToObject());
		args.GetReturnValue().Set(alcMakeContextCurrent(context->index));
	}
	void captureOpenDevice(const FunctionCallbackInfo<Value>& args) {
		Local<Object> result = newDevice(args);
		DeviceObject *obj = node::ObjectWrap::Unwrap<DeviceObject>(result->ToObject());
		char *str = getChar(args, 0);
		const int FREQ = 22050;
		obj->index = alcCaptureOpenDevice(str, FREQ, AL_FORMAT_MONO16, FREQ / 2);
		args.GetReturnValue().Set(result);
	}
	void captureStart(const FunctionCallbackInfo<Value>& args) {
		DeviceObject *device = node::ObjectWrap::Unwrap<DeviceObject>(args[0]->ToObject());
		alcCaptureStart(device->index);
	}
	void init(Local<Object> exports) {
		NODE_SET_METHOD(exports, "getError", getError);
		NODE_SET_METHOD(exports, "openDevice", openDevice);
		NODE_SET_METHOD(exports, "createContext", createContext);
		NODE_SET_METHOD(exports, "makeContext", makeContext);
		NODE_SET_METHOD(exports, "captureOpenDevice", captureOpenDevice);
		NODE_SET_METHOD(exports, "captureStart", captureStart);
		DeviceObject::Init(exports);
		ContextObject::Init(exports);
	}

	NODE_MODULE(openal, init)
}