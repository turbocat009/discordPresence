#include "ImgUtils.hpp"
#include <opencv2/opencv.hpp>

std::string modifyImg(const std::string &fileLoc) {
    cv::Mat img = cv::imread(fileLoc);

    if (img.empty()) {
        return "Failed to load image";
    }

    cv::Mat resized;
    cv::resize(img, resized, cv::Size(256, 256));

    std::vector<int> params = {
        cv::IMWRITE_JPEG_QUALITY, 70};

    cv::imwrite("outFile.jpg", resized, params);

    return "OK";
}