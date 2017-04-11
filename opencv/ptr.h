#ifdef _M_AMD64

#include <node.h>
#include <node_object_wrap.h>
#include <opencv2/opencv.hpp>

namespace opencv {
	class PtrObject : public node::ObjectWrap {
		public:
			cv::Ptr<cv::BackgroundSubtractorKNN> index;
			static v8::Persistent<v8::Function> constructor;
			static void Init(v8::Local<v8::Object> exports);

		private:
			explicit PtrObject();
			~PtrObject();
			static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
			static void Apply(const v8::FunctionCallbackInfo<v8::Value>& args);
	};
}

#endif