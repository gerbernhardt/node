#ifdef _M_AMD64

#include "ptr.h"
#include "mat.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

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

	Persistent<Function> PtrObject::constructor;
	PtrObject::PtrObject() {}
	PtrObject::~PtrObject() {}

	void PtrObject::Init(Local<Object> exports) {
		Isolate* isolate = exports->GetIsolate();
		Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
		tpl->SetClassName(String::NewFromUtf8(isolate, "Ptr"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		NODE_SET_PROTOTYPE_METHOD(tpl, "apply", Apply);
		constructor.Reset(isolate, tpl->GetFunction());
		exports->Set(String::NewFromUtf8(isolate, "Ptr"), tpl->GetFunction());
	}

	void PtrObject::New(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		if (args.IsConstructCall()) {
			PtrObject* obj = new PtrObject();
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

	void PtrObject::Apply(const FunctionCallbackInfo<Value>& args) {
		MatObject *image = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		MatObject *fgmask = node::ObjectWrap::Unwrap<MatObject>(args[1]->ToObject());
		double learningRate = args[2]->IsUndefined() ? -0.1 : args[2]->NumberValue();
		PtrObject *thisObj = node::ObjectWrap::Unwrap<PtrObject>(args.Holder());
		thisObj->index->apply(image->index, fgmask->index, learningRate);
	}
}

#endif