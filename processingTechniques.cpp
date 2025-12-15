#include "processingTechniques.hpp"
#include <cassert>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
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
//   ret, thresh = cv2.threshold(frame_8bit, 50, 255, cv2.THRESH_BINARY) #
//   threshhold step thresholded_fg.append(thresh)

// The full implementation
std::optional<std::vector<cv::Mat>>
backgroundModelling(const std::vector<cv::Mat> &v, double beta) {
  if (v.size() < 2) {
    return std::nullopt;
  }

  std::vector<cv::Mat> foregroundMasks;
  foregroundMasks.reserve(v.size());

  cv::Mat accumulator;
  v[0].convertTo(accumulator, CV_32F);

  for (size_t i = 0; i < v.size(); ++i) {
    cv::Mat currentFrameFloat;
    v[i].convertTo(currentFrameFloat, CV_32F);

    // Only update background where there's NO foreground motion

    cv::Mat diffFloat, diff8bit;
    cv::absdiff(currentFrameFloat, accumulator, diffFloat);

    diffFloat.convertTo(diff8bit, CV_8U);

    cv::Mat threshold_frame;
    cv::threshold(diff8bit, threshold_frame, 0, 255,
                  cv::THRESH_BINARY | cv::THRESH_OTSU);

    cv::Mat bgMask;
    cv::bitwise_not(threshold_frame, bgMask); // Invert: white = background
    cv::accumulateWeighted(currentFrameFloat, accumulator, beta, bgMask);
    foregroundMasks.push_back(threshold_frame);
  }

  return foregroundMasks;
}
std::optional<cv::Mat> applyErrosion(const cv::Mat &frame, int kernalSize) {
  cv::Mat result;
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,
                                             cv::Size(kernalSize, kernalSize));
  cv::erode(frame, result, kernel);
  if (result.empty()) {
    return std::nullopt;
  }
  return result;
}

std::optional<cv::Mat> applyDialation(const cv::Mat &frame, int kernalSize) {
  cv::Mat result;
  cv::Mat kernal = cv::getStructuringElement(cv::MORPH_RECT,
                                             cv::Size(kernalSize, kernalSize));
  cv::dilate(frame, result, kernal);
  if (result.empty()) {
    return std::nullopt;
  }
  return result;
}