#include "main.h"

namespace opencv {
	using v8::Context;
	using v8::Function;
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Number;
	using v8::Array;
	using v8::Object;
	using v8::Value;
	using v8::String;
	using v8::Handle;
	using v8::Persistent;
	/*
	* GENERAL FUNCTIONS
	*/
	bool getBool(const FunctionCallbackInfo<Value> &args, int index, bool default = false) {
		return args[index]->IsUndefined() ? default: args[index]->BooleanValue();
	}
	int getInt(const FunctionCallbackInfo<Value> &args, int index, int default = 0) {
		return (int)args[index]->IsUndefined() ? default: args[index]->Int32Value();
	}
	int getUInt(const FunctionCallbackInfo<Value> &args, int index, unsigned int default = 0) {
		return (unsigned int)args[index]->IsUndefined() ? default: args[index]->Uint32Value();
	}
	double getDouble(const FunctionCallbackInfo<Value> &args, int index, double default = 0.0) {
		return args[index]->IsUndefined() ? default : args[index]->NumberValue();
	}
	string getString(const FunctionCallbackInfo<Value> &args, int index) {
		String::Utf8Value utfValue(args[index]);
		return string(*utfValue);
	}
	char *getChar(const FunctionCallbackInfo<Value> &args, int index) {
		string str = getString(args, index);
		char *chr = new char[str.size() + 1];
		//char *chr = (char *)alloca(str.size() + 1);
		memcpy(chr, str.c_str(), str.size() + 1);
		return chr;
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
			} else result.push_back(def);
		}
		return result;
	}
	vector<double> getArrayDouble(const FunctionCallbackInfo<Value> &args, int index, int size, double def = 0.0) {
		vector<double> result;
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
				result.push_back(val->NumberValue());
			} else result.push_back(def);
		}
		return result;
	}
	Local<Object> newMat(const FunctionCallbackInfo<Value> &args) {
		Isolate *isolate = args.GetIsolate();
		const int argc = 1;
		Local<Value> argv[argc] = { args[0] };
		Local<Context> context = isolate->GetCurrentContext();
		Local<Function> cons = Local<Function>::New(isolate, MatObject::constructor);
		Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
		return result;
	}
#ifdef _M_AMD64
	Local<Object> newPtr(const FunctionCallbackInfo<Value> &args) {
		Isolate *isolate = args.GetIsolate();
		const int argc = 1;
		Local<Value> argv[argc] = { args[0] };
		Local<Context> context = isolate->GetCurrentContext();
		Local<Function> cons = Local<Function>::New(isolate, PtrObject::constructor);
		Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
		return result;
	}
#endif
	/*
	* API FUNCTIONS
	*/
	void blur(const FunctionCallbackInfo<Value> &args) {
		MatObject *src = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		MatObject *dst = node::ObjectWrap::Unwrap<MatObject>(args[1]->ToObject());
		vector<int> ksize = getArrayInt(args, 2, 2);
		vector<int> anchor = getArrayInt(args, 3, 2, -1);
		int borderType = getInt(args, 4, 4);
		cv::blur(src->index, dst->index, cv::Size(ksize[0], ksize[1]), cv::Point(anchor[0], anchor[1]), borderType);
		
	}
	void canny(const FunctionCallbackInfo<Value> &args) {
		MatObject *image = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		MatObject *edges = node::ObjectWrap::Unwrap<MatObject>(args[1]->ToObject());
		double thresholdd1 = getDouble(args, 2);
		double thresholdd2 = getDouble(args, 3);
		int apertureSize = getInt(args, 4, 3);
		bool L2gradient = getInt(args, 5);
		cv::Canny(image->index, edges->index, thresholdd1, thresholdd2, apertureSize, L2gradient);
	}
	void circle(const FunctionCallbackInfo<Value> &args) {
		MatObject *img = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		vector<int> center = getArrayInt(args, 1, 2);
		int radius = getInt(args, 2);
		vector<int> color = getArrayInt(args, 3, 3);
		int thickness = getInt(args, 4, 1);
		int lineType = getInt(args, 5, 8);
		int shift = getInt(args, 6, 0);
		cv::circle(img->index, cv::Point(center[0], center[1]), radius, cv::Scalar(color[0], color[1], color[2]), thickness, lineType, shift);
	}
#ifdef _M_AMD64
	void createBackgroundSubtractorKNN(const FunctionCallbackInfo<Value> &args) {
		int history = getInt(args, 0, 500);
		int dist2Threshold = getDouble(args, 1, 400.0);
		bool detectShadows = getBool(args, 2, true);
		Local<Object> result = newPtr(args);
		PtrObject *obj = node::ObjectWrap::Unwrap<PtrObject>(result->ToObject());
		obj->index = cv::createBackgroundSubtractorKNN(history, dist2Threshold, detectShadows);
		args.GetReturnValue().Set(result);
	}
#endif
	void cvtColor(const FunctionCallbackInfo<Value> &args) {
		MatObject *src = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		MatObject *dst = node::ObjectWrap::Unwrap<MatObject>(args[1]->ToObject());
		int code = getInt(args, 2);
		int dstCn = getInt(args, 3);
		cv::cvtColor(src->index, dst->index, code, dstCn);
	}
	void dilate(const FunctionCallbackInfo<Value> &args) {
		MatObject *src = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		MatObject *dst = node::ObjectWrap::Unwrap<MatObject>(args[1]->ToObject());
		MatObject *kernel = node::ObjectWrap::Unwrap<MatObject>(args[2]->ToObject());
		vector<int> anchor = getArrayInt(args, 3, 2, -1);
		int interations = getInt(args, 4, 1);
		int borderType = getInt(args, 5);
		cv::dilate(src->index, dst->index, kernel->index, cv::Point(anchor[0], anchor[1]), interations, borderType);
	}
	void erode(const FunctionCallbackInfo<Value> &args) {
		MatObject *src = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		MatObject *dst = node::ObjectWrap::Unwrap<MatObject>(args[1]->ToObject());
		MatObject *kernel = node::ObjectWrap::Unwrap<MatObject>(args[2]->ToObject());
		vector<int> anchor = getArrayInt(args, 3, 2, -1);
		int interations = getInt(args, 4, 1);
		int borderType = getInt(args, 5);
		cv::erode(src->index, dst->index, kernel->index, cv::Point(anchor[0], anchor[1]), interations, borderType);
	}
	void drawContours(const FunctionCallbackInfo<Value> &args) {
		MatObject *image = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		VectorObject *contours = node::ObjectWrap::Unwrap<VectorObject>(args[1]->ToObject());
		int contourIdx = getInt(args, 2);
		Handle<Array> array = Handle<Array>::Cast(args[3]);
		unsigned int r = array->Get(0)->IntegerValue();
		unsigned int g = array->Get(1)->IntegerValue();
		unsigned int b = array->Get(2)->IntegerValue();
		cv::Scalar color = cv::Scalar(b, g, r);
		int thickness = getInt(args, 4, 1);
		int lineType = getInt(args, 5, 8);
		vector<cv::Vec4i> hierarchy;
		int maxLevel = getInt(args, 7, 2147483647);
		vector<int> offset = getArrayInt(args, 2, 2);
		cv::drawContours(image->index, contours->index, contourIdx, color, thickness, lineType, hierarchy, maxLevel, cv::Point(offset[0], offset[1]));

	}
	void contourArea(const FunctionCallbackInfo<Value> &args) {
		Isolate *isolate = args.GetIsolate();
		VectorObject *contours = node::ObjectWrap::Unwrap<VectorObject>(args[1]->ToObject());
		int contourIndx = getInt(args, 1);
		bool oriented = getBool(args, 2);
		double value = cv::contourArea(contours->index[contourIndx], oriented);
		args.GetReturnValue().Set(Number::New(isolate, value));
	}
	void destroyAllWindows(const FunctionCallbackInfo<Value> &args) {
		cv::destroyAllWindows();
	}
	void destroyWindow(const FunctionCallbackInfo<Value> &args) {
		char *str = getChar(args, 0);
		cv::destroyWindow(str);
	}
	void findContours(const FunctionCallbackInfo<Value> &args) {
		MatObject *image = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		VectorObject *contours = node::ObjectWrap::Unwrap<VectorObject>(args[1]->ToObject());
		int mode = getInt(args, 2);
		int method = getInt(args, 3);
		vector<int> offset = getArrayInt(args, 4, 2, 0);
		cv::findContours(image->index, contours->index, mode, method, cv::Point(offset[0], offset[1]));

	}
	void flip(const FunctionCallbackInfo<Value> &args) {
		MatObject *src = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		MatObject *dst = node::ObjectWrap::Unwrap<MatObject>(args[1]->ToObject());
		int flipCode = getInt(args, 2);
		cv::flip(src->index, dst->index, flipCode);
	}
	void gaussianBlur(const FunctionCallbackInfo<Value> &args) {
		MatObject *src = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		MatObject *dst = node::ObjectWrap::Unwrap<MatObject>(args[1]->ToObject());
		vector<int> ksize = getArrayInt(args, 2, 2);
		double sigmaX = getDouble(args, 3);
		double sigmaY = getDouble(args, 4);
		int borderType = getInt(args, 5, 4);
		cv::GaussianBlur(src->index, dst->index, cv::Size(ksize[0], ksize[1]), sigmaX, sigmaY, borderType);
	}
	void getStructuringElement(const FunctionCallbackInfo<Value> &args) {
		Local<Object> result = newMat(args);
		MatObject *obj = node::ObjectWrap::Unwrap<MatObject>(result->ToObject());
		int shape = getInt(args, 0);
		vector<int> ksize = getArrayInt(args, 1, 2);
		vector<int> anchor = getArrayInt(args, 2, 2, -1);
		obj->index = cv::getStructuringElement(shape, cv::Size(ksize[0], ksize[1]), cv::Point(anchor[0], anchor[1]));
		args.GetReturnValue().Set(result);
	}
	void medianBlur(const FunctionCallbackInfo<Value> &args) {
		MatObject *src = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		MatObject *dst = node::ObjectWrap::Unwrap<MatObject>(args[1]->ToObject());
		int ksize = getInt(args, 2, 4);
		cv::medianBlur(src->index, dst->index, ksize);
	}
	void morphologyEx(const FunctionCallbackInfo<Value> &args) {
		MatObject *src = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		MatObject *dst = node::ObjectWrap::Unwrap<MatObject>(args[1]->ToObject());
		int op = getInt(args, 2);
		MatObject *kernel = node::ObjectWrap::Unwrap<MatObject>(args[3]->ToObject());
		vector<int> anchor = getArrayInt(args, 4, 2, -1);
		int interations = getInt(args, 5, 1);
		int borderType = getInt(args, 6);
		cv::morphologyEx(src->index, dst->index, op, kernel->index, cv::Point(anchor[0], anchor[1]), interations, borderType);
	}
	void namedWindow(const FunctionCallbackInfo<Value> &args) {
		char *str = getChar(args, 0);
		int flags = getInt(args, 1, 1);
		cv::namedWindow(str, flags);
	}
	void line(const FunctionCallbackInfo<Value> &args) {
		MatObject *img = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		vector<int> pt1 = getArrayInt(args, 1, 2);
		vector<int> pt2 = getArrayInt(args, 2, 2);
		vector<int> color = getArrayInt(args, 3, 3);
		int thickness = getInt(args, 4, 1);
		int lineType = getInt(args, 5, 8);
		int shift = getInt(args, 6, 0);
		cv::line(img->index, cv::Point(pt1[0], pt1[1]), cv::Point(pt2[0], pt2[1]), cv::Scalar(color[0], color[1], color[2]), thickness, lineType, shift);
	}
	void putText(const FunctionCallbackInfo<Value> &args) {
		MatObject *img = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		char *text = getChar(args, 1);
		vector<int> org = getArrayInt(args, 2, 2);
		int fontFace = getInt(args, 3);
		int fontScale = getInt(args, 4);
		vector<int> color = getArrayInt(args, 5, 3);
		int thickness = getInt(args, 6, 1);
		int lineType = getInt(args, 7, 8);
		bool bottomLeftOrigin = getBool(args, 8, 0);
		cv::putText(img->index, text, cv::Point(org[0], org[1]), fontFace, fontScale, cv::Scalar(color[0], color[1], color[2]), thickness, lineType, bottomLeftOrigin);
	}
	void rectangle(const FunctionCallbackInfo<Value> &args) {
		MatObject *img = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		vector<int> pt1 = getArrayInt(args, 1, 2);
		vector<int> pt2 = getArrayInt(args, 2, 2);
		vector<int> color = getArrayInt(args, 3, 3);
		int thickness = getInt(args, 4, 1);
		int lineType = getInt(args, 5, 8);
		int shift = getInt(args, 6, 0);
		cv::rectangle(img->index, cv::Point(pt1[0], pt1[1]), cv::Point(pt2[0], pt2[1]), cv::Scalar(color[0], color[1], color[2]), thickness, lineType, shift);
	}
	void resize(const FunctionCallbackInfo<Value> &args) {
		MatObject *src = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		MatObject *dst = node::ObjectWrap::Unwrap<MatObject>(args[1]->ToObject());
		vector<int> dsize = getArrayInt(args, 2, 0);
		double fx = getDouble(args, 3);
		double fy = getDouble(args, 4);
		int interpolation = getInt(args, 5, 1);
		cv::resize(src->index, dst->index, cv::Size(dsize[0], dsize[1]), fx, fy, interpolation);
	}
	void screenshot(const FunctionCallbackInfo<Value> &args) {
		// MAKE NEW CV::MAT
		Local<Object> result = newMat(args);
		MatObject *obj = node::ObjectWrap::Unwrap<MatObject>(result->ToObject());

		// GET ARGS
		HWND WindowHwnd = (HWND)getUInt(args, 0);
		DWORD left = getInt(args, 1), top = getInt(args, 2), width = getInt(args, 3), height = getInt(args, 4);

		RECT WindowParams;

		if (WindowHwnd == 0) WindowHwnd = GetDesktopWindow();
		GetWindowRect(WindowHwnd, &WindowParams);
		if (width == 0) width = WindowParams.right;
		if (height == 0) height = WindowParams.bottom;

		//CREATE
		HDC DevC = GetDC(NULL);
		HDC CaptureDC = CreateCompatibleDC(DevC);
		HBITMAP CaptureBitmap = CreateCompatibleBitmap(DevC, WindowParams.right - WindowParams.left, WindowParams.bottom - WindowParams.top);
		SelectObject(CaptureDC, CaptureBitmap);

		// BMP
		BITMAPINFOHEADER  bitmap;
		bitmap.biSize = sizeof(BITMAPINFOHEADER);
		bitmap.biWidth = width;
		bitmap.biHeight = height;
		bitmap.biPlanes = 1;
		bitmap.biBitCount = 32;
		bitmap.biCompression = BI_RGB;

		obj->index.create(height, width, CV_8UC4);
		BitBlt(CaptureDC, 0, 0, width, height, DevC, left, top, SRCCOPY | CAPTUREBLT);
		GetDIBits(CaptureDC, CaptureBitmap, 0, height, obj->index.data, (BITMAPINFO *)&bitmap, DIB_RGB_COLORS);

		cv::flip(obj->index, obj->index, 0);

		// CLEAN UP
		DeleteObject(CaptureBitmap);// fuck glitch 9994
		DeleteDC(CaptureDC);
		ReleaseDC(NULL, DevC);// fuck glitch 9994
		DeleteDC(DevC);
		// END

		args.GetReturnValue().Set(result);
	}
	void subtract(const FunctionCallbackInfo<Value> &args) {
		MatObject *src1 = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		MatObject *src2 = node::ObjectWrap::Unwrap<MatObject>(args[1]->ToObject());
		MatObject *dst = node::ObjectWrap::Unwrap<MatObject>(args[2]->ToObject());
		if (args[3]->IsUndefined()) {
			Local<Object> result = newMat(args);
			MatObject *mask = node::ObjectWrap::Unwrap<MatObject>(result->ToObject());
			int dtype = getInt(args, 4, -1);
			cv::subtract(src1->index, src2->index, dst->index, mask->index, dtype);
		} else {
			MatObject *mask = node::ObjectWrap::Unwrap<MatObject>(args[3]->ToObject());
			int dtype = getInt(args, 4, -1);
			cv::subtract(src1->index, src2->index, dst->index, mask->index, dtype);
		}
	}
	void threshold(const FunctionCallbackInfo<Value> &args) {
		MatObject *src = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		MatObject *dst = node::ObjectWrap::Unwrap<MatObject>(args[1]->ToObject());
		double thresh = getDouble(args, 2);
		double maxval = getDouble(args, 3);
		int type = getInt(args, 4);
		cv::threshold(src->index, dst->index, thresh, maxval, type);
	}
	void imread(const FunctionCallbackInfo<Value> &args) {
		Local<Object> result = newMat(args);
		MatObject *obj = node::ObjectWrap::Unwrap<MatObject>(result->ToObject());
		char *str = getChar(args, 0);
		obj->index = cv::imread(str);
		args.GetReturnValue().Set(result);
	}
	void imshow(const FunctionCallbackInfo<Value> &args) {
		char *str = getChar(args, 0);
		MatObject *obj = node::ObjectWrap::Unwrap<MatObject>(args[1]->ToObject());
		cv::imshow(str, obj->index);
	}
	void imwrite(const FunctionCallbackInfo<Value> &args) {
		char *str = getChar(args, 0);
		MatObject *obj = node::ObjectWrap::Unwrap<MatObject>(args[1]->ToObject());
		vector<int> params = getArrayInt(args, 2, 3);
		args.GetReturnValue().Set(cv::imwrite(str, obj->index, params));
	}
	void inRange(const FunctionCallbackInfo<Value> &args) {
		MatObject *src = node::ObjectWrap::Unwrap<MatObject>(args[0]->ToObject());
		vector<int> lowerb = getArrayInt(args, 1, 0);
		vector<int> upperb = getArrayInt(args, 2, 0);;
		MatObject *dst = node::ObjectWrap::Unwrap<MatObject>(args[3]->ToObject());
		cv::inRange(src->index, lowerb, upperb, dst->index);
	}
	void waitkey(const FunctionCallbackInfo<Value> &args) {
		int delay = getInt(args, 0);
		args.GetReturnValue().Set(cv::waitKey(delay));
	}
	void init(Local<Object> exports)  {
		NODE_SET_METHOD(exports, "blur", blur);
		NODE_SET_METHOD(exports, "canny", canny);
		NODE_SET_METHOD(exports, "circle", circle);
#ifdef _M_AMD64
		NODE_SET_METHOD(exports, "createBackgroundSubtractorKNN", createBackgroundSubtractorKNN);
#endif
		NODE_SET_METHOD(exports, "cvtColor", cvtColor);
		NODE_SET_METHOD(exports, "dilate", dilate);
		NODE_SET_METHOD(exports, "erode", erode);
		NODE_SET_METHOD(exports, "drawContours", drawContours);
		NODE_SET_METHOD(exports, "findContours", findContours);
		NODE_SET_METHOD(exports, "contourArea", contourArea);
		NODE_SET_METHOD(exports, "destroyAllWindows", destroyAllWindows);
		NODE_SET_METHOD(exports, "destroyWindow", destroyWindow);
		NODE_SET_METHOD(exports, "flip", flip);
		NODE_SET_METHOD(exports, "gaussianBlur", gaussianBlur);
		NODE_SET_METHOD(exports, "getStructuringElement", getStructuringElement);
		NODE_SET_METHOD(exports, "medianBlur", medianBlur);
		NODE_SET_METHOD(exports, "morphologyEx", morphologyEx);
		NODE_SET_METHOD(exports, "namedWindow", namedWindow);
		NODE_SET_METHOD(exports, "line", line);
		NODE_SET_METHOD(exports, "rectangle", rectangle);
		NODE_SET_METHOD(exports, "resize", resize);
		NODE_SET_METHOD(exports, "putText", putText);
		NODE_SET_METHOD(exports, "screenshot", screenshot);
		NODE_SET_METHOD(exports, "subtract", subtract);
		NODE_SET_METHOD(exports, "threshold", threshold);
		NODE_SET_METHOD(exports, "imread", imread);
		NODE_SET_METHOD(exports, "imshow", imshow);
		NODE_SET_METHOD(exports, "imwrite", imwrite);
		NODE_SET_METHOD(exports, "inRange", inRange);
		NODE_SET_METHOD(exports, "waitkey", waitkey);
		MatObject::Init(exports);
		VideoCaptureObject::Init(exports);
#ifdef _M_AMD64
		PtrObject::Init(exports);
#endif
		VectorObject::Init(exports);
	}
	
	NODE_MODULE(opencv, init)
}