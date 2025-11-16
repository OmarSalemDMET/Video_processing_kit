#ifndef UTILS_H
#define UTILS_H
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <optional>
#include <string>
#include <vector>


template <typename T>
void printVector(const std::vector<T>& v) {
    for (const auto& elem : v) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}


/// Wrapper for loading OpenCv Videos
cv::VideoCapture loadVideo(const std::string& path);
/// Returns the frames turned to grey scale
std::optional<std::vector<cv::Mat>> videoToGreyScale(cv::VideoCapture * vc);
/// Saves the video locally
void writeVideo(std::vector<cv::Mat> * v);
/// get weighted average
/// bg : Background 
/// cf : Current frame
inline cv::Mat getWeightedAverage(const double beta, const cv::Mat& bg, const cv::Mat& cf);

#endif