#ifdef _M_AMD64
	#pragma comment(lib, "node/Release64/node.lib")
#else
	#pragma comment(lib, "node/Release32/node.lib")
#endif

#include <node.h>
#include <iostream>
#include "myobject.h"

using namespace std;