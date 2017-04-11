#include <node.h>
#include <node_object_wrap.h>
#include <opencv2/opencv.hpp>
#include <vector>

namespace opencv {
	class VectorObject : public node::ObjectWrap {
		public:
			std::vector<std::vector<cv::Point>> index;
			static v8::Persistent<v8::Function> constructor;
			static void Init(v8::Local<v8::Object> exports);

		private:
			explicit VectorObject();
			~VectorObject();
			static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void Size(const v8::FunctionCallbackInfo<v8::Value>& args);
	};
}