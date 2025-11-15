#include "processingTechniques.hpp"


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