#ifndef PROCESSINGTECHNIQUES_HPP
#define PROCESSINGTECHNIQUES_HPP
#include <opencv2/core/mat.hpp>
#include <vector>
#include <opencv2/opencv.hpp>
#include <optional>





std::optional<std::vector<cv::Mat>> backgroundModelling(const std::vector<cv::Mat>& v, const double beta);
std::optional<std::vector<cv::Mat>> getFrameDifferance(std::vector<cv::Mat> * v);
std::optional<cv::Mat> applyErrosion(const cv::Mat &frame, int kernelSize);
std::optional<cv::Mat> applyDialation(const cv::Mat &frame, int kernalSize);

#endif