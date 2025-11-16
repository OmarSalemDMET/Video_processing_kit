#ifndef PROCESSINGTECHNIQUES_H
#define PROCESSINGTECHNIQUES_H
#include <vector>
#include <opencv2/opencv.hpp>
#include <optional>





std::optional<std::vector<cv::Mat>> backgroundModelling(const std::vector<cv::Mat>& v, const double beta);
std::optional<std::vector<cv::Mat>> getFrameDifferance(std::vector<cv::Mat> * v);




#endif