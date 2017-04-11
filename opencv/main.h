#ifdef _M_AMD64
	#pragma comment(lib, "node/Release64/node.lib")
	#pragma comment(lib, "opencv3.2/build/x64/vc14/lib/opencv_world320.lib")
#else
	#pragma comment(lib, "node/Release32/node.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_calib3d2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_contrib2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_core2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_features2d2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_flann2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_gpu2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_highgui2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_imgproc2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_legacy2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_ml2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_nonfree2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_objdetect2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_ocl2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_photo2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_stitching2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_superres2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_ts2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_video2413.lib")
	#pragma comment(lib, "opencv2.4/build/x86/vc12/lib/opencv_videostab2413.lib")
#endif

#define _CRT_SECURE_NO_WARNINGS 1

#include <node.h>
#include <node_object_wrap.h>
#include <opencv2/opencv.hpp>
#include "mat.h"
#include "videocapture.h"
#ifdef _M_AMD64
	#include "ptr.h"
#endif
#include "vector.h"
#include <windows.h>

using namespace std;