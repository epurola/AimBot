#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "ColorMasker.h"

HANDLE hSerial;

constexpr int REGION_WIDTH = 500;
constexpr int REGION_HEIGHT = 500;
const std::string SERIAL_PORT = "COM3";  // change to the actual port number

cv::Mat CaptureScreenRegion(int x, int y, int width, int height) {
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

//------------------------------
//Detect enemy in the OpenCV Mat
bool DetectTarget(const cv::Mat& frame, int& outX, int& outY) {
	
	ColorMasker masker;
	cv::Mat colorMask = masker.MaskPurple(frame);

	std::vector<std::vector<cv::Point>> contours;

	cv::findContours(colorMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// Find largest outline contour
	double maxArea = 0;
	cv::Rect largestBox;
	for (const auto& contour : contours) 
	{
		double area = cv::contourArea(contour);

		if (area > 50 && area > maxArea) 
		{
			maxArea = area;
			largestBox = cv::boundingRect(contour);
		}
	}

	if (maxArea > 0) 
	{
		cv::rectangle(frame, largestBox, cv::Scalar(0, 255, 0), 2);
		outX = largestBox.x + largestBox.width / 2;
		outY = largestBox.y + largestBox.height / 2;
		// Draw green circle at the center
		cv::circle(frame, cv::Point(outX, outY), 5, cv::Scalar(0, 255, 0), -1); 
		cv::Mat resizedFrame;
		cv::resize(frame, resizedFrame, cv::Size(), 1.5, 1.5); 

		cv::imshow("Screen Region", resizedFrame);

		cv::waitKey(1);
		return true;
	}
	cv::Mat resizedFrame;
	cv::resize(frame, resizedFrame, cv::Size(), 1.5, 1.5);

	cv::imshow("Screen Region", resizedFrame);
	cv::waitKey(1);
	
	if (contours.empty()) return false;

	return true;
}

//--------------------------

bool InitSerial(const std::string& portName) 
{
	std::string fullPort = "\\\\.\\" + portName;
	hSerial = CreateFileA(fullPort.c_str(), GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	if (hSerial == INVALID_HANDLE_VALUE) 
	{
		DWORD err = GetLastError();
		std::cerr << "Failed to open serial port. Error code: " << err << std::endl;
		return false;
	}
	DCB dcb = {};
	dcb.DCBlength = sizeof(dcb);
	if (!GetCommState(hSerial, &dcb)) return false;
	dcb.BaudRate = CBR_9600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	SetCommState(hSerial, &dcb);

	return true;
}

void SendAimOffset(int dx, int dy) 
{
	std::string msg = std::to_string(dx) + "," + std::to_string(dy) + "\n";
	DWORD written;
	WriteFile(hSerial, msg.c_str(), msg.size(), &written, NULL);
}


int main() 
{
	SetProcessDPIAware();
	int screenW = GetSystemMetrics(SM_CXSCREEN);
	int screenH = GetSystemMetrics(SM_CYSCREEN);

	//Top left corner of the capture region
	int centerX = screenW / 2 - REGION_WIDTH / 2;
	int centerY = screenH / 2 - REGION_HEIGHT / 2;

	//if (!InitSerial(SERIAL_PORT)) return 1;

	while (true) 
	{
		//Region around the center of the screen
		cv::Mat frame = CaptureScreenRegion(centerX, centerY, REGION_WIDTH, REGION_HEIGHT);

		int tx, ty; //Coordinates inside the region. The center of the contour bbox.
		if (DetectTarget(frame, tx, ty)) //Sets tx, ty values
		{
			int dx = tx - REGION_WIDTH / 2;
			int dy = ty - REGION_HEIGHT / 2;

			int screenX = centerX + tx;
			int screenY = centerY + ty;
			
			if (std::abs(dx) > 2 || std::abs(dy) > 2)
			{
				//SendAimOffset(screenX, screenY);
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
				//SetCursorPos(screenX, screenY);

			}
		}
	}
	CloseHandle(hSerial);
	return 0;
}
