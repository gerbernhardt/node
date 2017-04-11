#include "videocapture.h"
#include "mat.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace opencv {
	using v8::Context;
	using v8::Function;
	using v8::FunctionCallbackInfo;
	using v8::FunctionTemplate;
	using v8::Isolate;
	using v8::Local;
	using v8::Array;
	using v8::Uint32;
	using v8::Handle;
	using v8::Object;
	using v8::Persistent;
	using v8::String;
	using v8::Value;
	using v8::Number;

	Persistent<Function> VideoCaptureObject::constructor;
	VideoCaptureObject::VideoCaptureObject() {}
	VideoCaptureObject::~VideoCaptureObject() {}

	void VideoCaptureObject::Init(Local<Object> exports) {
		Isolate* isolate = exports->GetIsolate();
		Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
		tpl->SetClassName(String::NewFromUtf8(isolate, "VideoCapture"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		NODE_SET_PROTOTYPE_METHOD(tpl, "get", Get);
		NODE_SET_PROTOTYPE_METHOD(tpl, "grab", Grab);
		NODE_SET_PROTOTYPE_METHOD(tpl, "isOpened", IsOpened);
		NODE_SET_PROTOTYPE_METHOD(tpl, "open", Open);
		NODE_SET_PROTOTYPE_METHOD(tpl, "read", Read);
		NODE_SET_PROTOTYPE_METHOD(tpl, "release", Release);
		NODE_SET_PROTOTYPE_METHOD(tpl, "retrieve", Retrieve);
		NODE_SET_PROTOTYPE_METHOD(tpl, "set", Set);
		constructor.Reset(isolate, tpl->GetFunction());
		exports->Set(String::NewFromUtf8(isolate, "VideoCapture"), tpl->GetFunction());
	}

	void VideoCaptureObject::New(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		if (args.IsConstructCall()) {
			VideoCaptureObject* obj = new VideoCaptureObject();
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
	void VideoCaptureObject::Get(const FunctionCallbackInfo<Value>& args) {
		int propld = args[0]->Int32Value();
		VideoCaptureObject *thisObj = node::ObjectWrap::Unwrap<VideoCaptureObject>(args.Holder());
		args.GetReturnValue().Set(thisObj->index.get(propld));
	}
	void VideoCaptureObject::Grab(const FunctionCallbackInfo<Value>& args) {
		VideoCaptureObject *thisObj = node::ObjectWrap::Unwrap<VideoCaptureObject>(args.Holder());
		args.GetReturnValue().Set(thisObj->index.grab());
	}
	void VideoCaptureObject::IsOpened(const FunctionCallbackInfo<Value>& args) {
		VideoCaptureObject *thisObj = node::ObjectWrap::Unwrap<VideoCaptureObject>(args.Holder());
		args.GetReturnValue().Set(thisObj->index.isOpened());
	}
	void VideoCaptureObject::Open(const FunctionCallbackInfo<Value>& args) {
		VideoCaptureObject *thisObj = node::ObjectWrap::Unwrap<VideoCaptureObject>(args.Holder());
		if (!args[0]->IsUndefined()) {
			if (args[0]->IsString()) {
				String::Utf8Value utfValue(args[0]);
				string filename = string(*utfValue);
				args.GetReturnValue().Set(thisObj->index.open(filename));
			} else {
				int index = args[0]->Int32Value();
				args.GetReturnValue().Set(thisObj->index.open(index));
			}

		}
	}
	void VideoCaptureObject::Read(const FunctionCallbackInfo<Value>& args) {
		MatObject *image = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		VideoCaptureObject *thisObj = node::ObjectWrap::Unwrap<VideoCaptureObject>(args.Holder());
		args.GetReturnValue().Set(thisObj->index.read(image->index));
	}
	void VideoCaptureObject::Release(const FunctionCallbackInfo<Value>& args) {
		VideoCaptureObject *thisObj = node::ObjectWrap::Unwrap<VideoCaptureObject>(args.Holder());
		thisObj->index.release();
	}
	void VideoCaptureObject::Retrieve(const FunctionCallbackInfo<Value>& args) {
		MatObject *image = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		int flag = args[1]->IsUndefined() ? 0 : args[1]->Int32Value();
		VideoCaptureObject *thisObj = node::ObjectWrap::Unwrap<VideoCaptureObject>(args.Holder());
		args.GetReturnValue().Set(thisObj->index.retrieve(image->index, flag));
	}
	void VideoCaptureObject::Set(const FunctionCallbackInfo<Value>& args) {
		int propld = args[0]->Int32Value();
		double value = args[1]->NumberValue();
		VideoCaptureObject *thisObj = node::ObjectWrap::Unwrap<VideoCaptureObject>(args.Holder());
		args.GetReturnValue().Set(thisObj->index.set(propld, value));
	}
}