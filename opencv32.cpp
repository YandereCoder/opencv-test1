// opencv32.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include <fstream>
#include <random>
#include <algorithm>

using namespace std;
cv::Mat renderBW, renderBW_tresh;
int treshVal = 128;

void treshTrack(int val, void* ptr) {
	cv::threshold(renderBW, renderBW_tresh, treshVal, 255, CV_THRESH_BINARY);
}

int main()
{
	cv::Mat renderImg = cv::imread("C:\\walls\\tmp2\\futaribeyaRnd2.jpg", CV_LOAD_IMAGE_UNCHANGED);
	cv::Mat renderEdges;
	cv::Mat outerEdges;
	vector<vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;

	cv::resize(renderImg, renderImg, cv::Size(), 0.5, 0.5);
	outerEdges = cv::Mat::zeros(renderImg.size(), CV_8UC1);
	int keyVal;

	if (renderImg.rows > 1) {
		cv::cvtColor(renderImg, renderBW, CV_BGR2GRAY);
		cv::GaussianBlur(renderBW, renderBW, cv::Size(3, 3), 0, 0);
		cv::threshold(renderBW, renderBW, 254, 255, CV_THRESH_TOZERO_INV);

		cv::Canny(renderBW, renderEdges, 50, 120, 3);
		cv::threshold(renderEdges, renderEdges, 128, 255, CV_THRESH_BINARY);
		renderEdges.convertTo(renderEdges, CV_8UC1);

		// prepare trackbar and call tresh to initialize renderBW thresh
		//cv::findContours(renderEdges, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
		cv::namedWindow("BW");
		cv::imshow("BW", renderBW);
		cv::createTrackbar("Treshold #1", "BW", &treshVal, 255, treshTrack);
		treshTrack(treshVal, nullptr);

		for (int i = 0; i< contours.size(); i++)
		{
			//cv::Scalar color1 = cv::Scalar(255, 255, 255);
			drawContours(outerEdges, contours, i, 255, 1, 8, hierarchy, 0, cv::Point());
		}

		while(1) {
			cv::imshow("BW", renderBW);
			//int a = renderEdges.channels();
			cv::imshow("Edges", renderEdges);
			if (renderBW_tresh.rows > 1) {
				cv::imshow("OuterEdges", renderBW_tresh);
				cv::Canny(renderBW_tresh, renderEdges, 50, 120, 3);
			}

			keyVal = cv::waitKey(128);
			if (keyVal == (int)'q') {
				break;
			}
		}
		//system("pause");
	}

    return 0;
}

