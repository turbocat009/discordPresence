#include "ImgUtils.hpp"
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <iostream>
#include <filesystem>

std::string modifyImg(const std::string &fileLoc) {
    cv::Mat img = cv::imread(fileLoc);

    std::filesystem::path temp = std::filesystem::temp_directory_path();

    if (img.empty()) {
        return "Failed to load image";
    }

    cv::Mat resized;
    cv::resize(img, resized, cv::Size(256, 256));

    std::vector<int> params = {
        cv::IMWRITE_JPEG_QUALITY, 70};

    cv::imwrite(temp.string() + "outFile.jpg", resized, params);

    return temp.string() + "outFile.jpg";
}