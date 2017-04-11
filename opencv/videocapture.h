#include <node.h>
#include <node_object_wrap.h>
#include <opencv2/opencv.hpp>

namespace opencv {
	class VideoCaptureObject : public node::ObjectWrap {
		public:
			cv::VideoCapture index;
			static v8::Persistent<v8::Function> constructor;
			static void Init(v8::Local<v8::Object> exports);

		private:
			explicit VideoCaptureObject();
			~VideoCaptureObject();
			static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void Get(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void Grab(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void IsOpened(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void Open(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void Read(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void Release(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void Retrieve(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void Set(const v8::FunctionCallbackInfo<v8::Value>& args);
	};
}