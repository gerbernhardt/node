#include <node.h>
#include <node_object_wrap.h>
#include <opencv2/opencv.hpp>

namespace opencv {
	class MatObject : public node::ObjectWrap {
		public:
			cv::Mat index;
			static v8::Persistent<v8::Function> constructor;
			static void Init(v8::Local<v8::Object> exports);

		private:
			explicit MatObject();
			explicit MatObject(int rows, int cols, int type);
			~MatObject();
			static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void Clone(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void Empty(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void Size(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void Cols(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void Rows(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void GetCol(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void GetRow(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void GetPixel(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void SetPixel(const v8::FunctionCallbackInfo<v8::Value>& args);
	};
}