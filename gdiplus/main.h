#ifdef _M_AMD64
	#pragma comment(lib, "node/Release64/node.lib")
	#pragma comment (lib,"Gdiplus.lib")
#else
	#pragma comment(lib, "node/Release32/node.lib")
	#pragma comment (lib,"Gdiplus.lib")
#endif

#include <windows.h>
#include <gdiplus.h> // gdiplus.h up to node.h, else error!!!! :S
#include <node.h>

using namespace std;
using namespace Gdiplus;
