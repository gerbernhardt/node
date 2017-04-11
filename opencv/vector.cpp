#include "vector.h"

namespace opencv {
	using v8::Context;
	using v8::Function;
	using v8::FunctionCallbackInfo;
	using v8::FunctionTemplate;
	using v8::Isolate;
	using v8::Local;
	using v8::Array;
	using v8::Object;
	using v8::Persistent;
	using v8::String;
	using v8::Value;

	Persistent<Function> VectorObject::constructor;
	VectorObject::VectorObject() {}
	VectorObject::~VectorObject() {}

	void VectorObject::Init(Local<Object> exports) {
		Isolate* isolate = exports->GetIsolate();
		Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
		tpl->SetClassName(String::NewFromUtf8(isolate, "Vector"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		NODE_SET_PROTOTYPE_METHOD(tpl, "size", Size);
		constructor.Reset(isolate, tpl->GetFunction());
		exports->Set(String::NewFromUtf8(isolate, "Vector"), tpl->GetFunction());
	}

	void VectorObject::New(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		if (args.IsConstructCall()) {
			VectorObject* obj = new VectorObject();
			obj->Wrap(args.This());
			args.GetReturnValue().Set(args.This());
		} else {
			const int argc = 1;
			Local<Value> argv[argc] = { args[0] };
			Local<Context> context = isolate->GetCurrentContext();
			Local<Function> cons = Local<Function>::New(isolate, constructor);
			Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
			args.GetReturnValue().Set(result);
		}
	}
	void VectorObject::Size(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		VectorObject *thisObj = node::ObjectWrap::Unwrap<VectorObject>(args.Holder());
		args.GetReturnValue().Set((unsigned int)thisObj->index.size());
	}
}