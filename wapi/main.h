#ifdef _M_AMD64
	#pragma comment(lib, "node/Release64/node.lib")
#else
	#pragma comment(lib, "node/Release32/node.lib")
#endif

#include <windows.h>
#include <tchar.h>
#include <Tlhelp32.h>
#include <iostream>
#include <string>
#include <iostream>
#include <node.h>
#include "sendkeys.h"

using namespace std;