#include "processingTechniques.hpp"
#include <cassert>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include "utils.hpp"
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <optional>
#include <vector>


std::optional<std::vector<cv::Mat>>
getFrameDifferance(std::vector<cv::Mat> *v) {

  if (v->empty()) {
    std::cerr << "Early Escape, empty video.\n";
    return std::nullopt;
  }

  // Moving frame result/output
  std::vector<cv::Mat> mv_result;

  for (int i = 0; i < (v->size() - 1); i++) {
    cv::Mat f = v->at(i + 1) - v->at(i);
    mv_result.push_back(f);
  }

  return mv_result;
}

// thresholded_fg = []
// for frame in fg:
//   # Convert to 8-bit unsigned integer type before thresholding
//   frame_8bit = cv2.convertScaleAbs(frame)
//   ret, thresh = cv2.threshold(frame_8bit, 50, 255, cv2.THRESH_BINARY) # threshhold step
//   thresholded_fg.append(thresh)

std::optional<std::vector<cv::Mat>> backgroundModelling(
        const std::vector<cv::Mat>& v,
        double beta)
{
    if (v.size() < 2) {
        return std::nullopt;  
    }

    std::vector<cv::Mat> fg;
    fg.reserve(v.size() - 1);

    for (size_t i = 0; i < v.size() - 1; ++i) {
        const cv::Mat& bg = (i == 0) ? v[i] : fg[i - 1];
        const cv::Mat& cf = v[i + 1];

        // ensure proper copy of result
        fg.push_back(getWeightedAverage(beta, bg, cf));
    }
    //t_fg = threshold foreground 
    std::vector<cv::Mat> t_fg;

    for(int i=0; i<fg.size(); i++){
      cv::Mat frame_8_bit;
      cv::convertScaleAbs(fg.at(i), frame_8_bit);
      cv::Mat threshold_vid;
      cv::threshold(frame_8_bit, threshold_vid, 50, 255, cv::THRESH_BINARY_INV);
      t_fg.push_back(threshold_vid);
    }

    return t_fg;
}

std::optional<cv::Mat> applyErrosion(const cv::Mat &frame, int kernalSize){
  cv::Mat result;
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernalSize, kernalSize));
  cv::erode(frame, result, kernel);
  if (result.empty()){
    return std::nullopt;
  }
  return result;
}

std::optional<cv::Mat> applyDialation(const cv::Mat &frame, int kernalSize){
  cv::Mat result; 
  cv::Mat kernal = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernalSize, kernalSize));
  cv::dilate(frame, result, kernal);
  if (result.empty()){
    return std::nullopt;
  }

  return result; 

}