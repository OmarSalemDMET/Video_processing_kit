#include "processingTechniques.hpp"
#include <opencv2/core/mat.hpp>
#include "utils.cpp"
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


std::optional<std::vector<cv::Mat>> backgroundModelling(const std::vector<cv::Mat> &v, const double beta){
  std::vector<cv::Mat> fg;
  for(int i = 0; i < v.size(); i++){
    if (i == 0){
      cv::Mat bg = v.at(i);
      cv::Mat cf = v.at(i + 1);
      fg.push_back(getWeightedAverage(beta, bg, cf));
    }
    else {
            cv::Mat bg = fg.at(i - 1);
      cv::Mat cf = v.at(i + 1);
      fg.push_back(getWeightedAverage(beta, bg, cf));
    }
  }
  return fg;
}