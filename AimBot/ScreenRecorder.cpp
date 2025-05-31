#include "ScreenRecorder.h"

ScreenRecorder::ScreenRecorder(int width, int height)
	:width(width), height(height)
{
	SetProcessDPIAware();
	int screenW = GetSystemMetrics(SM_CXSCREEN);
	int screenH = GetSystemMetrics(SM_CYSCREEN);

	//Top left corner of the capture region
	x = screenW / 2 - width / 2;
	y = screenH / 2 - height / 2;
}

cv::Mat ScreenRecorder::CaptureScreen()
{
	HDC hScreenDC = GetDC(NULL);
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

	HBITMAP hBitMap = CreateCompatibleBitmap(hScreenDC, width, height);
	SelectObject(hMemoryDC, hBitMap);

	BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, x, y, SRCCOPY);

	BITMAPINFOHEADER bi = {};
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height; //- to flip image the right side up
	bi.biPlanes = 1;
	bi.biBitCount = 24;
	bi.biCompression = BI_RGB;

	cv::Mat mat(height, width, CV_8UC3);
	GetDIBits(hMemoryDC, hBitMap, 0, height, mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	DeleteObject(hBitMap);
	DeleteDC(hMemoryDC);
	DeleteDC(hScreenDC);

	return mat;
}
