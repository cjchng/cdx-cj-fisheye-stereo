#pragma once
#include <opencv2/core.hpp>
#include <string>
#include <vector>

namespace stereo {
struct Dataset {
    cv::Size image_size, board_size; // board_size counts INNER corners
    double square_size = 0;
    std::string unit;
    std::vector<std::vector<cv::Point2d>> left, right;
};
struct Result {
    cv::Mat K1, D1, K2, D2, R, T, camera2_center_in_camera1;
    double rms_left, rms_right, rms_stereo, baseline;
};
void validate(const Dataset& data);
std::vector<cv::Point3d> boardPoints(const Dataset& data);
Dataset readDataset(const cv::FileNode& root);
void writeDataset(cv::FileStorage& fs, const Dataset& data);
// Estimates intrinsics separately, then holds them fixed during stereo fitting.
// Throws std::invalid_argument for input errors, cv::Exception for solver errors.
Result calibrate(const Dataset& data);
void writeResult(cv::FileStorage& fs, const Dataset& data, const Result& result);
std::string resultJson(const Dataset& data, const Result& result);
}
