#include  "main.h"

ULONG_PTR token;
GdiplusStartupInput input;
GdiplusStartupOutput output;
Graphics *graphics;

namespace gdiplus {
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::Number;
	using v8::Value;
	using v8::String;
	using v8::Array;
	using v8::Handle;

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
		//char *chr = (char *)alloca(str.size() + 1);
		memcpy(chr, str.c_str(), str.size() + 1);
		return chr;
	}
	bool getBool(const FunctionCallbackInfo<Value>& args, int index) {
		return args[index]->IsUndefined() ? false : args[index]->BooleanValue();
	}
	int getInt(const FunctionCallbackInfo<Value>& args, int index) {
		return (int)args[index]->IsUndefined() ? 0 : args[index]->Int32Value();
	}
	int getFloat(const FunctionCallbackInfo<Value>& args, int index) {
		return (float)args[index]->IsUndefined() ? 0 : args[index]->NumberValue();
	}
	int getUInt(const FunctionCallbackInfo<Value>& args, int index) {
		return (unsigned int)args[index]->IsUndefined() ? 0 : args[index]->Uint32Value();
	}
	vector<int> getArrayInt(const FunctionCallbackInfo<Value> &args, int index, int size, int def = 0) {
		vector<int> result;
		if (args[index]->IsUndefined() || !args[index]->IsArray()) {
			for (unsigned int i = 0; i < size; i++)
				result.push_back(def);
			return result;
		}
		Handle<Value> val;
		Local<Array> arr;
		Handle<Array> jsArray = Handle<Array>::Cast(args[index]);
		if (size == 0) size = jsArray->Length();
		for (unsigned int i = 0; i < size; i++) {
			if (i < jsArray->Length()) {
				val = jsArray->Get(i);
				result.push_back(val->IntegerValue());
			}
			else result.push_back(def);
		}
		return result;
	}

	/*
	 * API FUNCTIONS
	 */
	void drawLine(const FunctionCallbackInfo<Value>& args) {
		vector<int> color = getArrayInt(args, 0, 4);
		float border = getFloat(args, 1);
		Pen pen(Color(color[0], color[1], color[2], color[3]), border);
		int x1 = getInt(args, 2);
		int y1 = getInt(args, 3);
		int x2 = getInt(args, 4);
		int y2 = getInt(args, 5);
		graphics->DrawLine(&pen, x1, y1, x2, y2);
	}
	void drawRectangle(const FunctionCallbackInfo<Value>& args) {
		vector<int> color = getArrayInt(args, 0, 4);
		float border = getFloat(args, 1);
		Pen pen(Color(color[0], color[1], color[2], color[3]), border);
		int x = getInt(args, 2);
		int y = getInt(args, 3);
		int width = getInt(args, 4);
		int height = getInt(args, 5);
		graphics->DrawRectangle(&pen, x, y, width, height);
	}
	void fromHDC(const FunctionCallbackInfo<Value>& args) {
		HDC hdc = (HDC)getUInt(args, 0);
		graphics = Graphics::FromHDC(hdc);
	}
	void fromHWND(const FunctionCallbackInfo<Value>& args) {
		HWND hwnd = (HWND)getUInt(args, 0);
		graphics = Graphics::FromHWND(hwnd);
	}
	void gdiplusStartup(const FunctionCallbackInfo<Value>& args) {
		GdiplusStartup(&token, &input, &output);
	}
	void releaseHDC(const FunctionCallbackInfo<Value>& args) {
		HDC hdc = (HDC)getUInt(args, 0);
		graphics->ReleaseHDC(hdc);
	}

	void init(Local<Object> exports)  {
		NODE_SET_METHOD(exports, "drawLine", drawLine);
		NODE_SET_METHOD(exports, "drawRectangle", drawRectangle);
		NODE_SET_METHOD(exports, "fromHDC", fromHDC);
		NODE_SET_METHOD(exports, "fromHWND", fromHWND);
		NODE_SET_METHOD(exports, "gdiplusStartup", gdiplusStartup);
		NODE_SET_METHOD(exports, "releaseHDC", releaseHDC);
	}

	NODE_MODULE(gdiplus, init)
}
