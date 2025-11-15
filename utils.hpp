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
/// Turn video, in-place, to grey scale for 
/// less memery usage.
/// WARNING: this function mutates the original 
/// Video data 
std::optional<std::vector<cv::Mat>> videoToGreyScale(cv::VideoCapture * vc);
void writeVideo(std::vector<cv::Mat> * v);
#endif