/*
 * homography.hpp
 *
 *  Created on: Feb 13, 2013
 *      Author: dodo
 */

#ifndef HOMOGRAPHY_HPP_
#define HOMOGRAPHY_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <vector>
#include <string>
#include <iostream>

class Homography {
private:
	std::vector<cv::Point2f> srcPoints; //kinect view
	std::vector<cv::Point2f> dstPoints; //field template

public:
	Homography(std::vector<cv::Point2f> srcPoints, std::vector<cv::Point2f> dstPoints);
	~Homography();
	cv::Mat calcHomography();
	void draw(const cv::Mat& srcImage, const cv::Mat& dstImage, const cv::Mat& H);
private:
	void calcProjection(cv::Point2f &point, const cv::Mat &H, double &x, double &y);
};


#endif /* HOMOGRAPHY_HPP_ */
