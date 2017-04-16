#include "main.h"

namespace wapi {
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::Number;
	using v8::Value;
	using v8::String;

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
	int getUInt(const FunctionCallbackInfo<Value>& args, int index) {
		return (unsigned int)args[index]->IsUndefined() ? 0 : args[index]->Uint32Value();
	}


	/*
	 * API FUNCTIONS
	 */
	void setWindowPos(const FunctionCallbackInfo<Value>& args) {
		HWND handle = (HWND)getUInt(args, 0);
		HWND afterInsert = (HWND)getUInt(args,1);
		int left = getInt(args, 2);
		int top = getInt(args, 3);
		int width = getInt(args, 4);
		int height = getInt(args, 5);
		int flags = getInt(args, 6);
		args.GetReturnValue().Set(SetWindowPos(handle, afterInsert, left, top, width, height, flags));
	}

	void getWindowRect(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		HWND handle = (HWND)getUInt(args, 0);
		RECT window;
		Local<Object> obj = Object::New(isolate);
		if (GetWindowRect(handle, &window)) {
			obj->Set(String::NewFromUtf8(isolate, "left"), Number::New(isolate, window.left));
			obj->Set(String::NewFromUtf8(isolate, "top"), Number::New(isolate, window.top));
			obj->Set(String::NewFromUtf8(isolate, "right"), Number::New(isolate, window.right));
			obj->Set(String::NewFromUtf8(isolate, "bottom"), Number::New(isolate, window.bottom));
			obj->Set(String::NewFromUtf8(isolate, "width"), Number::New(isolate, window.right - window.left));
			obj->Set(String::NewFromUtf8(isolate, "height"), Number::New(isolate, window.bottom - window.top));
		}
		args.GetReturnValue().Set(obj);
	}

	void findWindow(const FunctionCallbackInfo<Value>& args) {
		char *str = getChar(args, 0);
		args.GetReturnValue().Set((long)FindWindow(NULL, str));
	}
	void getActiveWindow(const FunctionCallbackInfo<Value>& args) {
		args.GetReturnValue().Set((long)GetActiveWindow());
	}
	void getConsoleWindow(const FunctionCallbackInfo<Value>& args) {
		args.GetReturnValue().Set((long)GetConsoleWindow());
	}
	void getDesktopWindow(const FunctionCallbackInfo<Value>& args) {
		args.GetReturnValue().Set((long)GetDesktopWindow());
	}
	void getForegroundWindow(const FunctionCallbackInfo<Value>& args) {
		args.GetReturnValue().Set((long)GetForegroundWindow());
	}
	void getDC(const FunctionCallbackInfo<Value>& args) {
		HWND handle = (HWND)getUInt(args, 0);
		args.GetReturnValue().Set((long)GetDC(handle));
	}
	void enableWindow(const FunctionCallbackInfo<Value>& args) {
		HWND handle = (HWND)getUInt(args, 0);
		bool enable = getBool(args, 1);
		args.GetReturnValue().Set(EnableWindow(handle, enable));
	}

	void setActiveWindow(const FunctionCallbackInfo<Value>& args) {
		HWND handle = (HWND)getUInt(args, 0);
		args.GetReturnValue().Set((long)SetActiveWindow(handle)); // SOLO SI LA VENTANA ESTA EN PRIMER PLANO
	}
	void setForegroundWindow(const FunctionCallbackInfo<Value>& args) {
		HWND handle = (HWND)getUInt(args, 0);
		args.GetReturnValue().Set(SetForegroundWindow(handle)); // FUERZA A LA VENTANA AL PRIMER PLANO
	}
	void setFocus(const FunctionCallbackInfo<Value>& args) {
		HWND handle = (HWND)getUInt(args, 0);
		args.GetReturnValue().Set((long)SetFocus(handle));
	}
	void getListProcessId(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
		PROCESSENTRY32 pEntry;
		pEntry.dwSize = sizeof(pEntry);
		BOOL hRes = Process32First(hSnapShot, &pEntry);
		Local<Object> obj = Object::New(isolate);
		while (hRes) {
			obj->Set(String::NewFromUtf8(isolate, pEntry.szExeFile), Number::New(isolate, pEntry.th32ProcessID));
			hRes = Process32Next(hSnapShot, &pEntry);
		}
		CloseHandle(hSnapShot);
		args.GetReturnValue().Set(obj);
		// RETORNA UN ARRAY CON LOS PROCESOS CON SU PID: FIREFOX.EXE=>9899
	}

	void getWindowThreadProcessId(const FunctionCallbackInfo<Value>& args) {
		HWND handle = (HWND)getUInt(args, 0);
		DWORD pid;
		GetWindowThreadProcessId((HWND)handle, &pid);
		// RETORNA EL PID DEL PROCESO PASANDOLE EL MANEJADOR DE LA VENTANA
		args.GetReturnValue().Set((long)pid);
	}
	void openProcess(const FunctionCallbackInfo<Value>& args) {
		DWORD pid = (DWORD)getUInt(args, 0);
		args.GetReturnValue().Set((long)OpenProcess(PROCESS_ALL_ACCESS, false, pid));
	}
	void closeHandle(const FunctionCallbackInfo<Value>& args) {
		HANDLE handle = (HANDLE)getUInt(args, 0);
		args.GetReturnValue().Set(CloseHandle(handle));
	}

	void terminateProcess(const FunctionCallbackInfo<Value>& args) {
		HANDLE handle = (HANDLE)getUInt(args, 0);
		args.GetReturnValue().Set(TerminateProcess(handle, 9));
	}

	void suspendProcess(const FunctionCallbackInfo<Value>& args) {
		HANDLE handle = (HANDLE)getUInt(args, 0);
		typedef LONG(NTAPI *NtSuspendProcess)(IN HANDLE handle);
		NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(GetModuleHandle("ntdll"), "NtSuspendProcess");
		args.GetReturnValue().Set(pfnNtSuspendProcess(handle));
	}

	void resumeProcess(const FunctionCallbackInfo<Value>& args) {
		HANDLE handle = (HANDLE)getUInt(args, 0);
		typedef LONG(NTAPI *NtResumeProcess)(IN HANDLE handle);
		NtResumeProcess pfnNtResumeProcess = (NtResumeProcess)GetProcAddress(GetModuleHandle("ntdll"), "NtResumeProcess");
		args.GetReturnValue().Set(pfnNtResumeProcess(handle));
	}
	void readProcessMemory(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		HANDLE handle = (HANDLE)getUInt(args, 0);
		LPCVOID address = (LPCVOID)getUInt(args, 1);
		LPCVOID longBuffer;
		char *str= getChar(args, 2);
		char strBuffer[64];
		if (strcmp(str, "str") == 0) {
			ReadProcessMemory(handle, address, &strBuffer, sizeof(strBuffer), 0);
			args.GetReturnValue().Set(String::NewFromUtf8(isolate, strBuffer));
		} else {
			ReadProcessMemory(handle, address, &longBuffer, sizeof(longBuffer), 0);
			args.GetReturnValue().Set((long)longBuffer);
		}
	}
	void writeProcessMemory(const FunctionCallbackInfo<Value>& args) {
		HANDLE handle = (HANDLE)getUInt(args, 0);
		LPVOID address = (LPVOID)getUInt(args, 1);
		LPVOID value = (LPVOID)getUInt(args, 2);
		SIZE_T bytes;
		args.GetReturnValue().Set(WriteProcessMemory(handle, address, &value, sizeof(value), &bytes));
	}
	void getWindowText(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		HWND handle = (HWND)getUInt(args, 0);
		char *text = getChar(args, 1);
		GetWindowText(handle, text, sizeof(text));
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, text));
	}
	void setWindowText(const FunctionCallbackInfo<Value>& args) {
		HWND handle = (HWND)getUInt(args, 0);
		char *text = getChar(args, 1);
		args.GetReturnValue().Set(SetWindowText((HWND)handle, (LPCSTR)text));	
	}
	void getClipboard(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		if (!OpenClipboard(NULL)) {
			args.GetReturnValue().Set(NULL);
		} else {
			HANDLE handle = GetClipboardData(CF_TEXT);
			CloseClipboard();
			args.GetReturnValue().Set(String::NewFromUtf8(isolate, (char*)handle));
		}
	}

	void setClipboard(const FunctionCallbackInfo<Value>& args)  {
		char *data = getChar(args, 0);
		HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE, strlen(data) + 1);
		memcpy(GlobalLock(hMem), data, strlen(data) + 1);
		GlobalUnlock(hMem);
		OpenClipboard(NULL);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hMem);
		CloseClipboard();
	}

	void mouseEvent(const FunctionCallbackInfo<Value>& args)  {
		DWORD event = (DWORD)getUInt(args, 0);
		DWORD x = (DWORD)getUInt(args, 1);
		DWORD y = (DWORD)getUInt(args, 2);
		DWORD data = (DWORD)getUInt(args, 3);
		DWORD info = (DWORD)getUInt(args, 4);
		mouse_event(event, x, y, data, info);
	}

	void getCursorPos(const FunctionCallbackInfo<Value>& args)  {
		POINT point;
		Isolate *isolate = args.GetIsolate();
		if (GetCursorPos(&point))  {
			Local<Object> obj = Object::New(isolate);
			obj->Set(String::NewFromUtf8(isolate, "x"), Number::New(isolate, point.x));
			obj->Set(String::NewFromUtf8(isolate, "y"), Number::New(isolate, point.y));
			args.GetReturnValue().Set(obj);
		}
	}

	void setCursorPos(const FunctionCallbackInfo<Value>& args)  {
		int x = getInt(args, 0);
		int y = getInt(args, 0);
		SetCursorPos(x, y);
	}

	void getKeyState(const FunctionCallbackInfo<Value>& args) {
		int key = getInt(args, 0);
		if (GetKeyState(key) & 0x80)
			args.GetReturnValue().Set(true);
		else args.GetReturnValue().Set(false);
	}

	void dialog(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		char *action = getChar(args, 0);
		char *filter = getChar(args, 1);

		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = 0;
		ofn.lpstrDefExt = 0;
		ofn.lpstrFile = new TCHAR[MAX_PATH];
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = _T(filter);
		ofn.nFilterIndex = 0;
		ofn.lpstrInitialDir = 0;
		ofn.lpstrTitle = 0;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		/*
		CHOOSECOLOR cc;
		COLORREF crCustColors[16];
		COLORREF old_attr;
		ZeroMemory(&cc, sizeof(cc));
		cc.lStructSize = sizeof(CHOOSECOLOR);
		//cc.rgbResult = old_attr;
		cc.lpCustColors = crCustColors;
		cc.Flags = CC_RGBINIT | CC_FULLOPEN;
		ChooseColor(&cc);
		args.GetReturnValue().Set((int)cc.rgbResult);
		*/
		if (strcmp(action, "open") == 0)
			GetOpenFileName(&ofn);
		else GetSaveFileName(&ofn);

		args.GetReturnValue().Set(String::NewFromUtf8(isolate, ofn.lpstrFile));
	}

	CSendKeys m_sk;// SendKeys Object
	void sendkeys(const FunctionCallbackInfo<Value>& args) {
		char *str = getChar(args, 0);
		m_sk.SendKeys(_T(str), true);
	}

	void getLine(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		string str;
		getline(cin, str);
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, str.c_str()));
	}
	void sleep(const FunctionCallbackInfo<Value>& args)  {
		Sleep((DWORD)getUInt(args, 0) * 1000);
	}

	void usleep(const FunctionCallbackInfo<Value>& args) {
		LARGE_INTEGER li;
		li.QuadPart = -10 * getUInt(args, 0);     // negative values for relative time
		HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
		SetWaitableTimer(timer, &li, 0, NULL, NULL, 0);
		WaitForSingleObject(timer, INFINITE);
		CloseHandle(timer);
	}
	void testGet(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		unsigned int long long i = 0x000000A2ED6B1580;
		printf("Get int %I64d\n", i);
		std::string str = to_string(i);
		printf("Get str %s\n", str);
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, str.c_str()));
	}
	void testSet(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		string x = getString(args, 0);
		printf("Set str %s\n", x);
		unsigned int long long i = stoi(x.c_str());
		printf("Set int %I64d\n", i);
	}
	void init(Local<Object> exports)  {
		NODE_SET_METHOD(exports, "testGet", testGet);
		NODE_SET_METHOD(exports, "testSet", testSet);
		NODE_SET_METHOD(exports, "setWindowPos", setWindowPos);
		NODE_SET_METHOD(exports, "getWindowRect", getWindowRect);
		NODE_SET_METHOD(exports, "findWindow", findWindow);
		NODE_SET_METHOD(exports, "getActiveWindow", getActiveWindow);
		NODE_SET_METHOD(exports, "getConsoleWindow", getConsoleWindow);
		NODE_SET_METHOD(exports, "getDesktopWindow", getDesktopWindow);
		NODE_SET_METHOD(exports, "getForegroundWindow", getForegroundWindow);
		NODE_SET_METHOD(exports, "getDC", getDC);
		NODE_SET_METHOD(exports, "enableWindow", enableWindow);
		NODE_SET_METHOD(exports, "setActiveWindow", setActiveWindow);
		NODE_SET_METHOD(exports, "setForegroundWindow", setForegroundWindow);
		NODE_SET_METHOD(exports, "setFocus", setFocus);
		 // process 
		NODE_SET_METHOD(exports, "getListProcessId", getListProcessId);
		NODE_SET_METHOD(exports, "getWindowThreadProcessId", getWindowThreadProcessId);
		NODE_SET_METHOD(exports, "openProcess", openProcess);
		NODE_SET_METHOD(exports, "closeHandle", closeHandle);
		NODE_SET_METHOD(exports, "terminateProcess", terminateProcess);
		NODE_SET_METHOD(exports, "suspendProcess", suspendProcess);
		NODE_SET_METHOD(exports, "resumeProcess", resumeProcess);
		// memory
		NODE_SET_METHOD(exports, "readProcessMemory", readProcessMemory);
		NODE_SET_METHOD(exports, "writeProcessMemory", writeProcessMemory);

		NODE_SET_METHOD(exports, "getWindowText", getWindowText);
		NODE_SET_METHOD(exports, "setWindowText", setWindowText);
		
		NODE_SET_METHOD(exports, "getClipboard", getClipboard);
		NODE_SET_METHOD(exports, "setClipboard", setClipboard);
		
		NODE_SET_METHOD(exports, "mouseEvent", mouseEvent);
		
		NODE_SET_METHOD(exports, "getCursorPos", getCursorPos);
		NODE_SET_METHOD(exports, "setCursorPos", setCursorPos);
		
		NODE_SET_METHOD(exports, "getKeyState", getKeyState);

		NODE_SET_METHOD(exports, "dialog", dialog);
		
		NODE_SET_METHOD(exports, "sendkeys", sendkeys);

		NODE_SET_METHOD(exports, "getLine", getLine);
		NODE_SET_METHOD(exports, "sleep", sleep);
		NODE_SET_METHOD(exports, "usleep", usleep);
	}

	NODE_MODULE(wapi, init)
}
