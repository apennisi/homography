#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "homography.hpp"

std::vector<cv::Point2f> mapPoints;
std::vector<cv::Point2f> rgbPoints;

cv::Mat rgb;
cv::Mat map;

void mouseHandlerRobot(int event, int x, int y, int flags, void *param) {
    if(event == CV_EVENT_LBUTTONDOWN) {
        cv::circle(map, cv::Point2f(x, y), 3, cv::Scalar(0, 0, 255), -1);
        mapPoints.push_back(cv::Point2f(x, y));
        cv::imshow("map", map);
    }

    if(event == CV_EVENT_RBUTTONDOWN) {
        if(mapPoints.size() == rgbPoints.size()) {
            Homography h(rgbPoints, mapPoints);
            cv::Mat H = h.calcHomography();
            h.draw(rgb, map, H);
            std::cout << H << std::endl;
        } else {
            std::cout << "ERRORE" << std::endl;
        }
    }
}

void mouseHandlerRGB(int event, int x, int y, int flags, void *param) {
    if(event == CV_EVENT_LBUTTONDOWN) {
        cv::circle(rgb, cv::Point2f(x, y), 3, cv::Scalar(0, 0, 255), -1);
        rgbPoints.push_back(cv::Point2f(x, y));
        cv::imshow("rgb", rgb);
    }

    if(event == CV_EVENT_RBUTTONDOWN) {
        if(mapPoints.size() == rgbPoints.size()) {
            Homography h(rgbPoints, mapPoints);
            cv::Mat H = h.calcHomography();
            h.draw(rgb, map, H);
            std::cout << H << std::endl;
        } else {
            std::cout << "ERRORE" << std::endl;
        }
    }
}



int main(int argc, char **argv) {

    if(argc != 3) {
        std::cout << "ERROR: " << argv[0] << " rgb_image map_image" << std::endl;
        exit(-1);
    }

    rgb = cv::imread(argv[1]);
    map = cv::imread(argv[2]);


    cv::namedWindow("map", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("rgb", CV_WINDOW_AUTOSIZE);

    cv::setMouseCallback("map", &mouseHandlerRobot, NULL);
    cv::setMouseCallback("rgb", &mouseHandlerRGB, NULL);


    cv::imshow("rgb", rgb);
    cv::imshow("map", map);

    cv::waitKey(0);

}
