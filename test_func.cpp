#include "processingTechniques.hpp"
#include "utils.hpp"
#include <iostream>
#include <opencv2/videoio.hpp>
#include <vector>




void testFrameDifference(const std::string& videoPath){
    cv::VideoCapture vid = loadVideo(videoPath);
    std::vector<cv::Mat> greyFrames;
    if (auto result = videoToGreyScale(&vid)){
        greyFrames = *result;
    }else{
        std::cerr<<"Couldn't convert frames to grey scale\n";
    }
    std::vector<cv::Mat> proccessedVid;
    if (auto result = getFrameDifferance(&greyFrames)){
        proccessedVid = *result;
    }else{
        std::cerr<<"Couldn't get frame difference\n";
    }
    writeVideo(&proccessedVid, "./output.mp4");
}