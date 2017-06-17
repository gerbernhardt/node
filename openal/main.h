#ifdef _M_AMD64
	#pragma comment(lib, "node/Release64/node.lib")
	#pragma comment(lib, "openal1.1/libs/Win64/OpenAL32.lib")
#else
	#pragma comment(lib, "node/Release32/node.lib")
	#pragma comment(lib, "openal1.1/libs/Win32/OpenAL32.lib")
#endif

#include <node.h>
#include <windows.h>
#include <iostream>
#include <openal1.1/include/al.h>
#include <openal1.1/include/alc.h>
#include "device.h"
#include "context.h"

using namespace std;