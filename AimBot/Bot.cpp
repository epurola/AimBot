#include <algorithm>
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "ColorMasker.h"
#include "EdgeDetector.h"
#include "EnemyDetector.h"
#include "ScreenRecorder.h"

HANDLE hSerial;

constexpr int REGION_WIDTH = 500;
constexpr int REGION_HEIGHT = 500;
const std::string SERIAL_PORT = "COM3";  // change to the actual port number


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
	if (!InitSerial(SERIAL_PORT)) return 1;
	
	//Detects enemies based on (provided color not yet implemented) color
	EnemyDetector enemyDetector;
	//Records area in the middle of the screen which size is defined by REGION_WIDTH and REGION_HEIGHT
	ScreenRecorder screenRecorder(REGION_WIDTH, REGION_HEIGHT);

	while (true) 
	{
		//Region around the center of the screen
		cv::Mat frame = screenRecorder.CaptureScreen();
		//Returns the bigges box whiich is likely the enemy
		cv::Rect bbox = enemyDetector.detectEnemy(frame);

		int tx, ty; //Coordinates inside the recorded region. 
		if (bbox.area() > 0) 
		{
			//Center of the enemy box
			tx = bbox.x + bbox.width / 2;
		    ty = bbox.y + bbox.height / 2;

			//Draw bbox around the enemy and a circle to visualize aim region
			//Aim may not always be on the enemy todo: make sure it stays inside detected area
			cv::rectangle(frame, bbox, cv::Scalar(0, 255, 0), 2);
			cv::circle(frame, cv::Point(tx, ty), 5, cv::Scalar(0, 255, 0), -1);

			//Location of the enemy on the sreen
			int screenX = screenRecorder.GetX() + tx;
			int screenY = screenRecorder.GetY() + ty;

			POINT currentPos;
			GetCursorPos(&currentPos);

			//aim delta
			int dx = screenX - currentPos.x;
			int dy = screenY - currentPos.y;
		
			if (std::abs(dx) > 1 || std::abs(dy) > 1)
			{
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
					//SetCursorPos(screenX, screenY);
					SendAimOffset(dx, dy);
			}
		}
		cv::Mat resizedFrame;
		cv::resize(frame, resizedFrame, cv::Size(), 1.5, 1.5);

		cv::imshow("Screen Region", resizedFrame);
		cv::waitKey(1);
	}

	CloseHandle(hSerial);
	return 0;
}
