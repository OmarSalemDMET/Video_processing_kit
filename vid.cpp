#include "vid.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <optional>
#include <stdexcept>
#include <vector>



cv::VideoCapture loadVideo(const std::string& path){
   cv::VideoCapture vc(path);
   if (!vc.isOpened()) {
    throw std::invalid_argument("Couldn't open file of path : " + path);
   }
   std::cout<<"Video Opened Successfully\n";

   return vc ;
}

std::optional<std::vector<cv::Mat>>videoToGreyScale(cv::VideoCapture* vc){
    std::vector<cv::Mat> result;
    while (vc->isOpened()){
        cv::Mat frame;
        bool ret = vc->read(frame);
        if(!ret) return std::nullopt;
        cv::Mat greyFrame;
        cv::cvtColor(frame, greyFrame, cv::COLOR_BGR2GRAY);
        result.push_back(greyFrame);
    }

    printVector(result);
    return result;
}