#include "utils.hpp"
#include <opencv2/calib3d.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <optional>
#include <stdexcept>
#include <tuple>
#include <vector>

cv::VideoCapture loadVideo(const std::string &path) {
  cv::VideoCapture vc(path);
  if (!vc.isOpened()) {
    throw std::invalid_argument("Couldn't open file of path : " + path);
  }
  std::cout << "Video Opened Successfully\n";

  return vc;
}

std::optional<std::vector<cv::Mat>> videoToGreyScale(cv::VideoCapture *vc) {
  std::vector<cv::Mat> result;
  if (!vc->isOpened()) {
    std::cerr
        << "Error: videoToGreyScale was given an unopened VideoCapture.\n";
    return std::nullopt;
  }
  while (true) {
    cv::Mat frame;
    bool ret = vc->read(frame);
    if (!ret)
      break;
    if (frame.empty()) {
      std::cerr << "Warning: Read an empty frame, stopping.\n";
      break;
    }
    cv::Mat greyFrame;
    cv::cvtColor(frame, greyFrame, cv::COLOR_BGR2GRAY);
    result.push_back(greyFrame);
  }

  return result;
}

void writeVideo(std::vector<cv::Mat> *v, const std::string &path) {
  if (v->empty()) {
    std::cerr << "Error: Cannot write empty video.\n";
    return;
  }

  int f_width = v->at(0).cols;
  int f_height = v->at(0).rows;

  // VideoWriter: MP4 container with MP4V codec
  cv::VideoWriter out(path, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), 24.0,
                      cv::Size(f_width, f_height),
                      true // expects 3-channel BGR frames
  );

  if (!out.isOpened()) {
    std::cerr << "Error: Could not open output.mp4 for writing.\n";
    return;
  }

  for (size_t i = 0; i < v->size(); i++) {
    cv::Mat frame = v->at(i);

    // If frame is grayscale → convert back to BGR for saving
    if (frame.channels() == 1) {
      cv::Mat bgr;
      cv::cvtColor(frame, bgr, cv::COLOR_GRAY2BGR);
      out.write(bgr);
    } else {
      out.write(frame);
    }
  }

  out.release();
  std::cout << "Video successfully written to output.mp4\n";
}

cv::Mat getWeightedAverage(double beta, const cv::Mat &bg, const cv::Mat &cf) {
  cv::Mat result;
  result = beta * cf + (1.0 - beta) * bg; // OpenCV handles weighted add
  return result;
}

std::tuple<std::vector<std::vector<cv::Point>>, std::vector<cv::Rect>>
centroidTracker(const cv::Mat &frame) {
  std::vector<std::vector<cv::Point>> contours;
  // since the frames comes in black and white already find contours directly
  cv::findContours(frame, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  std::vector<cv::Rect> blobs(contours.size());
  std::vector<std::vector<cv::Point>> centroids;
  for (size_t i = 0; i < contours.size(); i++) {
    blobs[i] = cv::boundingRect(contours[i]);
    std::vector<cv::Point> centroid;
    centroid.push_back(cv::Point(blobs[i].x + blobs[i].width / 2,
                                 blobs[i].y + blobs[i].height / 2));
    centroids.push_back(centroid);
  }
  return std::make_tuple(centroids, blobs);
}
