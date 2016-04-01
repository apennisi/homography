/*
 * homography.cpp
 *
 *  Created on: Feb 13, 2013
 *      Author: dodo
 */

#include "homography.hpp"

Homography::Homography(std::vector<cv::Point2f> srcPoints, std::vector<cv::Point2f> dstPoints) {
	std::vector<cv::Point2f>::const_iterator itPts = srcPoints.begin();
	for( ; itPts != srcPoints.end(); ++itPts ) {
		this->srcPoints.push_back(*itPts);
	}
	itPts = dstPoints.begin();
	for( ; itPts != dstPoints.end(); ++itPts ) {
		this->dstPoints.push_back(*itPts);
	}
}

Homography::~Homography() {
	delete(&srcPoints);
	delete(&dstPoints);
}

cv::Mat Homography::calcHomography() {

	std::cout << "\nsrc points: " << std::endl;
	std::vector<cv::Point2f>::const_iterator itPts = srcPoints.begin();
	for( ; itPts != srcPoints.end(); ++itPts ) {
		std::cout << (*itPts) << "\n";
	}

	std::cout << "\ndst points: " << std::endl;
	itPts = dstPoints.begin();
	for( ; itPts != dstPoints.end(); ++itPts ) {
		std::cout << (*itPts) << "\n";
	}

	cv::Mat H = cv::findHomography(
						cv::Mat(srcPoints), cv::Mat(dstPoints),  // corresponding points
						//inliers,	// outputed inliers matches
						0, 			//force all corrispondences to be used
						0.5);	    // max distance to reprojection point

	std::cout << std::endl;
	std::cout << "MATRIX H" << std::endl;
	for (int i = 0; i < H.rows; i++) {
		for (int j = 0; j < H.cols; j++) {
			std::cout << H.at<double>(i,j) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

    cv::Mat invH = H.inv();

    std::cout << std::endl;
    std::cout << "INV MATRIX H" << std::endl;
    for (int i = 0; i < invH.rows; i++) {
        for (int j = 0; j < invH.cols; j++) {
            std::cout << invH.at<double>(i,j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

	return H;
}


void Homography::draw(const cv::Mat& srcImage, const cv::Mat& dstImage, const cv::Mat& H) {
	cv::Mat dstMat;
	dstImage.copyTo(dstMat);
	cv::Mat srcMat;
	srcImage.copyTo(srcMat);


	// Draw the inlier points
	std::vector<cv::Point2f>::const_iterator itPts = dstPoints.begin();

	while (itPts != dstPoints.end()) {
		cv::circle(dstMat,*itPts, 6, cv::Scalar(120,120,120), 2);
		++itPts;
	}

	itPts = srcPoints.begin();
	while (itPts != srcPoints.end()) {
		cv::circle(srcMat,*itPts, 6, cv::Scalar(120,120,120), 2);
		++itPts;
	}

	// Display the images with points
	cv::namedWindow("Dst Points");
	cv::imshow("Dst Points", dstMat);
	cv::namedWindow("Src Points");
	cv::imshow("Src Points",srcMat);

    //cv::waitKey(0);


	//reprojection

	std::cout << "Reprojection..." << std::endl;

	cv::Mat reprojectionImage = dstMat.clone();
	itPts = srcPoints.begin();

	int cnt = 0;
	for(; itPts != srcPoints.end(); ++itPts) {

		double x, y;
		cv::Point2f p((*itPts).x, (*itPts).y);
		calcProjection(p, H, x, y);

		std::cout << "(" << p.x << "," << p.y << ") --> (" << x << "," << y << ")" << std::endl;

		if(x < 0 || y < 0) {
			std::cout << "WARNING: Negative reprojection values" << std::endl;
		}
		else {
			cv::line(reprojectionImage, cv::Point2f(x, y),
				cv::Point2f(x+10, y), CV_RGB(0,0,255), 2);
			cv::line(reprojectionImage, cv::Point2f(x, y),
				cv::Point2f(x-10, y), CV_RGB(0,0,255), 2);
			cv::line(reprojectionImage, cv::Point2f(x, y),
				cv::Point2f(x, y+10), CV_RGB(0,0,255), 2);
			cv::line(reprojectionImage, cv::Point2f(x, y),
				cv::Point2f(x, y-10), CV_RGB(0,0,255), 2);

			std::string s;
			std::stringstream out;
			out << cnt;
			s.assign(out.str());
			int s_size = s.size();
			cv::putText(reprojectionImage, s, cv::Point2f(x+5,y-5),
						CV_FONT_HERSHEY_SIMPLEX, 0.6, CV_RGB(0, 0, 255));
		}
	}

	cv::namedWindow("Reprojected Points");
	cv::imshow("Reprojected Points", reprojectionImage);

    //cv::waitKey(0);

	cv::Mat warpImage = cv::Mat::zeros(dstMat.size(), dstMat.type());
	cv::warpPerspective(srcImage, warpImage, H, dstMat.size());

	cv::namedWindow("Warp Image");
	cv::imshow("Warp Image", warpImage);

	cv::waitKey(0);
}

void Homography::calcProjection(cv::Point2f &point, const cv::Mat &H, double &x, double &y) {
	x =	(point.x*H.at<double>(0,0)+point.y*H.at<double>(0,1)+H.at<double>(0,2))/
			(point.x*H.at<double>(2,0)+point.y*H.at<double>(2,1)+H.at<double>(2,2));
	y =	(point.x*H.at<double>(1,0)+point.y*H.at<double>(1,1)+H.at<double>(1,2))/
			(point.x*H.at<double>(2,0)+point.y*H.at<double>(2,1)+H.at<double>(2,2));
}



