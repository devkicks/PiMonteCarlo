#include "opencv\cv.h"
#include "opencv\highgui.h"
#include <stdio.h>

#define C_WIDTH 600
#define C_TEXTSIZE 40

cv::Vec2f drawRandomSample()
{
	cv::Vec2f outVec;
	outVec(1) = std::rand() % C_WIDTH;
	outVec(0) = std::rand() % C_WIDTH;

	return outVec;
}

int main()
{
	cv::Mat displayImage;

	displayImage = cv::Mat::zeros(C_WIDTH + C_TEXTSIZE, C_WIDTH, CV_8UC3);

	int ptsInside = 0;
	int ptsOutside = 0;
	int savedImageCount = 0;
	for (int i = 0; i < 20000; i++)
	{
		cv::Vec2f randSample = drawRandomSample();

		//std::cout << randSample << std::endl;

		// equation of circle
		// r^2 = (a - C_WIDTH/2)^2 + (b - C_WIDTH/2)^2

		double radiusCircle = C_WIDTH / 2;
		double testSample = std::pow(randSample(0) - radiusCircle, 2) + std::pow(randSample(1) - radiusCircle, 2);
		double radiusSquare = std::pow(radiusCircle, 2);

		if (testSample < radiusSquare)
		{
			// inside the circle
			ptsInside++;
			displayImage.at<cv::Vec3b>(randSample(1), randSample(0)) = cv::Vec3b(0, 255, 0);
		}
		else
		{
			//outside the cicle
			ptsOutside++;
			displayImage.at<cv::Vec3b>(randSample(1), randSample(0)) = cv::Vec3b(0, 0, 255);
		}

		double piCalculated = ((double)ptsInside / ((double)ptsInside + (double)ptsOutside)) * 4;

		//std::cout << "Value of Pi:" << piCalculated << std::endl;

		cv::Point textLoc;
		textLoc.x = 0;
		textLoc.y = (int)(C_WIDTH + C_TEXTSIZE / 1.2);
		char buffer[100];
		std::sprintf(buffer, "Pts In: %d | Pts Out: %d | # Iter: %d | Calc Pi: %f", ptsInside, ptsOutside, i, piCalculated);
		cv::putText(displayImage, buffer, textLoc, CV_FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(255, 255, 255));
		cv::imshow("Display Image", displayImage);

		// save image
		if (i % 100 == 0)
		{
			// save a screenshot of the simulation
			char buf[50];
			std::sprintf(buf, "savedGifs\\image%0.4d.png", savedImageCount++);
			std::cout << buf << std::endl;
			cv::imwrite(buf, displayImage);
		}

		// clear text area
		for (int jj = C_WIDTH; jj < C_WIDTH + C_TEXTSIZE; jj++)
			for (int ii = 0; ii < C_WIDTH; ii++)
				displayImage.at<cv::Vec3b>(jj, ii) = cv::Vec3b(0, 0, 0);
		char esc = cv::waitKey(1);
		if (esc == 27)
			exit(0);
	}

	cv::waitKey(0);
	return 0;
}