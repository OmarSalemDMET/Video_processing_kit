#include "processingTechniques.hpp"
#include "utils.hpp"
#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <vector>

void testFrameDifference(const std::string &videoPath) {
  cv::VideoCapture vid = loadVideo(videoPath);
  std::vector<cv::Mat> greyFrames;
  if (auto result = videoToGreyScale(&vid)) {
    greyFrames = *result;
  } else {
    std::cerr << "Couldn't convert frames to grey scale\n";
  }
  std::vector<cv::Mat> proccessedVid;
  if (auto result = getFrameDifferance(&greyFrames)) {
    proccessedVid = *result;
  } else {
    std::cerr << "Couldn't get frame difference\n";
  }
  writeVideo(&proccessedVid, "./output.mp4");
}

void testBackgroundModelling(const std::string &videoPath) {
  cv::VideoCapture vid = loadVideo(videoPath);
  std::vector<cv::Mat> greyFrames;
  if (auto result = videoToGreyScale(&vid)) {
    greyFrames = *result;
  } else {
    std::cerr << "Couldn't get grey frames\n";
  }
  std::vector<cv::Mat> proccessedVid;
  if (auto result = backgroundModelling(greyFrames, 0.3)) {
    proccessedVid = *result;
  } else {
    std::cerr << "Couldn't preform background modelling\n";
  }
  writeVideo(&proccessedVid, "./output_bg_modelling.mp4");
}

void testCentroidTracker(const std::string &videoPath) {
  // 1. Load Video
  cv::VideoCapture vid = loadVideo(videoPath);

  // 2. Convert to Grayscale
  std::vector<cv::Mat> greyFrames;
  if (auto result = videoToGreyScale(&vid)) {
    greyFrames = *result;
  } else {
    std::cerr << "Couldn't get grey frames\n";
    return;
  }

  // 3. Background Modelling (Returns Binary Masks now)
  std::vector<cv::Mat> fgMasks;
  if (auto result = backgroundModelling(greyFrames, 1)) {
    fgMasks = *result;
  } else {
    std::cerr << "Background modelling failed\n";
    return;
  }

  std::vector<cv::Mat> processedFrames;

  // 4 & 5. Noise Reduction (Erosion) + Connecting Blobs (Dilation)
  // - Erosion (3x3) 
  // - Dilation (15x15)
  for (const auto &frame : fgMasks) {
    // Apply Erosion
    auto erodedOpt = applyErrosion(frame, 6);
    if (!erodedOpt)
      continue;

    // Apply Dilation on the eroded frame
    auto dilatedOpt = applyDialation(*erodedOpt, 15);
    if (!dilatedOpt)
      continue;

    processedFrames.push_back(*dilatedOpt);
  }

  // 6. Tracking & Visualization
  // Reset video to read color frames for final output
  vid.set(cv::CAP_PROP_POS_FRAMES, 0);
  std::vector<cv::Mat> originalFrames;
  cv::Mat temp;
  while (vid.read(temp))
    originalFrames.push_back(temp.clone());

  // Debug: Save the black/white masks to see what the tracker "sees"
  writeVideo(&processedFrames, "./output_debug_masks.mp4");

  std::vector<cv::Mat> outputFrames = originalFrames;
  std::vector<double> timestamps;
  double fps = vid.get(cv::CAP_PROP_FPS);

  // Loop through processed binary frames and track
  for (size_t i = 0; i < processedFrames.size() && i < outputFrames.size();
       i++) {
    double timestamp = i / fps;
    timestamps.push_back(timestamp);

    // Pass the CLEAN BINARY MASK to the tracker
    auto [centroids, blobs, ts] =
        centroidTracker(processedFrames[i], timestamp);

    // Draw results on the COLOR frame
    addBlobsToFrame(outputFrames[i], blobs, timestamps);
  }

  writeVideo(&outputFrames, "./output_centroid_tracking.mp4");
}