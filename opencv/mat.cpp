#include "mat.h"
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
	using v8::Uint32;
	using v8::Handle;
	using v8::Object;
	using v8::Persistent;
	using v8::String;
	using v8::Value;

	Persistent<Function> MatObject::constructor;

	MatObject::MatObject() {
		this->index = Mat();
	}
	MatObject::MatObject(int rows, int cols, int type) {
		this->index = Mat(rows, cols, type);
	}
	MatObject::~MatObject() {}

	void MatObject::Init(Local<Object> exports) {
		Isolate* isolate = exports->GetIsolate();
		Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
		tpl->SetClassName(String::NewFromUtf8(isolate, "Mat"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		NODE_SET_PROTOTYPE_METHOD(tpl, "clone", Clone);
		NODE_SET_PROTOTYPE_METHOD(tpl, "empty", Empty);
		NODE_SET_PROTOTYPE_METHOD(tpl, "size", Size);
		NODE_SET_PROTOTYPE_METHOD(tpl, "cols", Cols);
		NODE_SET_PROTOTYPE_METHOD(tpl, "rows", Rows);
		NODE_SET_PROTOTYPE_METHOD(tpl, "getCol", GetCol);
		NODE_SET_PROTOTYPE_METHOD(tpl, "getRow", GetRow);
		NODE_SET_PROTOTYPE_METHOD(tpl, "getPixel", GetPixel);
		NODE_SET_PROTOTYPE_METHOD(tpl, "setPixel", SetPixel);
		constructor.Reset(isolate, tpl->GetFunction());
		exports->Set(String::NewFromUtf8(isolate, "Mat"), tpl->GetFunction());
	}

	void MatObject::New(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		if (args.IsConstructCall()) {
			if (args[0]->IsUndefined()) {
				MatObject *obj = new MatObject();
				obj->Wrap(args.This());
				args.GetReturnValue().Set(args.This());
			} else {
				int rows = args[0]->Uint32Value();
				int cols = args[1]->Uint32Value();
				int type = args[2]->Uint32Value();
				MatObject *obj = new MatObject(rows, cols, type);
				obj->Wrap(args.This());
				args.GetReturnValue().Set(args.This());
			}
		} else {
			const int argc = 1;
			Local<Value> argv[argc] = { args[0] };
			Local<Context> context = isolate->GetCurrentContext();
			Local<Function> cons = Local<Function>::New(isolate, constructor);
			Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
			args.GetReturnValue().Set(result);
		}
	}
	void MatObject::Clone(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		const int argc = 1;
		Local<Value> argv[argc] = { args[0] };
		Local<Context> context = isolate->GetCurrentContext();
		Local<Function> cons = Local<Function>::New(isolate, MatObject::constructor);
		Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
		
		MatObject *newObj = node::ObjectWrap::Unwrap<MatObject>(result->ToObject());
		MatObject *thisObj = node::ObjectWrap::Unwrap<MatObject>(args.Holder());
		
		newObj->index = thisObj->index.clone();

		args.GetReturnValue().Set(result);
	}
	void MatObject::Empty(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		MatObject *thisObj = node::ObjectWrap::Unwrap<MatObject>(args.Holder());
		args.GetReturnValue().Set(thisObj->index.empty());
	}
	void MatObject::Size(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		MatObject *thisObj = node::ObjectWrap::Unwrap<MatObject>(args.Holder());
		Local<Object> obj = Object::New(isolate);
		obj->Set(String::NewFromUtf8(isolate, "width"), Uint32::New(isolate, thisObj->index.size().width));
		obj->Set(String::NewFromUtf8(isolate, "height"), Uint32::New(isolate, thisObj->index.size().height));
		args.GetReturnValue().Set(obj);
	}
	void MatObject::Cols(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		MatObject *thisObj = node::ObjectWrap::Unwrap<MatObject>(args.Holder());
		args.GetReturnValue().Set(thisObj->index.cols);
	}
	void MatObject::Rows(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		MatObject *thisObj = node::ObjectWrap::Unwrap<MatObject>(args.Holder());
		args.GetReturnValue().Set(thisObj->index.rows);
	}
	void MatObject::GetCol(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		MatObject *thisObj = node::ObjectWrap::Unwrap<MatObject>(args.Holder());
		unsigned int x = args[0]->Uint32Value();
		int rows = thisObj->index.col(x).rows;
		Local<Array> array = Array::New(isolate, rows);
		for (int i = 0; i < rows; i++) {
			Local<Array> subArray = Array::New(isolate, 3);
			Vec3b pixel = thisObj->index.at<Vec3b>(x, i);
			subArray->Set(0, Uint32::New(isolate, (float)pixel[2]));
			subArray->Set(1, Uint32::New(isolate, (float)pixel[1]));
			subArray->Set(2, Uint32::New(isolate, (float)pixel[0]));
			array->Set(i, subArray);
		}
		args.GetReturnValue().Set(array);
	}
	void MatObject::GetRow(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		MatObject *thisObj = node::ObjectWrap::Unwrap<MatObject>(args.Holder());
		unsigned int y = args[0]->Uint32Value();
		int cols = thisObj->index.row(y).cols;
		Local<Array> array = Array::New(isolate, cols);
		for (int i = 0; i < cols; i++) {
			Local<Array> subArray = Array::New(isolate, 3);
			Vec3b pixel = thisObj->index.at<Vec3b>(i, y);
			subArray->Set(0, Uint32::New(isolate, (float)pixel[2]));
			subArray->Set(1, Uint32::New(isolate, (float)pixel[1]));
			subArray->Set(2, Uint32::New(isolate, (float)pixel[0]));
			array->Set(i, subArray);
		}
		args.GetReturnValue().Set(array);
	}
	void MatObject::GetPixel(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		MatObject *thisObj = node::ObjectWrap::Unwrap<MatObject>(args.Holder());
		unsigned int x = args[0]->Uint32Value();
		unsigned int y = args[1]->Uint32Value();
		
		Vec3b pixel = thisObj->index.at<Vec3b>(x, y);
		Local<Array> array = Array::New(isolate, 3);
		array->Set(0, Uint32::New(isolate, (float)pixel[2]));
		array->Set(1, Uint32::New(isolate, (float)pixel[1]));
		array->Set(2, Uint32::New(isolate, (float)pixel[0]));
		args.GetReturnValue().Set(array);
	}
	void MatObject::SetPixel(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		MatObject *thisObj = node::ObjectWrap::Unwrap<MatObject>(args.Holder());
		unsigned int x = args[0]->Uint32Value();
		unsigned int y = args[1]->Uint32Value();
		Handle<Array> array = Handle<Array>::Cast(args[2]);
		unsigned int r = array->Get(0)->IntegerValue();
		unsigned int g = array->Get(1)->IntegerValue();
		unsigned int b = array->Get(2)->IntegerValue();
		thisObj->index.row(x).col(y) = Scalar(b, g, r);
	}
}