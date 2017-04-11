#define CONTEXT_H

#include <node.h>
#include <node_object_wrap.h>
#include <openal1.1\include\al.h>
#include <openal1.1\include\alc.h>

namespace openal {
	class ContextObject : public node::ObjectWrap {
	public:
		ALCcontext *index;
		static v8::Persistent<v8::Function> constructor;
		static void Init(v8::Local<v8::Object> exports);

	private:
		explicit ContextObject();
		~ContextObject();
		static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
	};

}